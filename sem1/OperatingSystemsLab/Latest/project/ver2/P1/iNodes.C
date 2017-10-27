/*
 * inodes.C of CEG4350:OS File Sys Project This code is "a little too
 * complete" for the purposes of P0.  Every thing here is useful in
 * P1.

 * prabhaker.mateti@wright.edu
 */

#include "fstypes.h"

/* pre:: fv->super partially initialized, iHeight includes file-size
 * field, iHeight >= 3 ;; post:: Construct the inodes on the disk. */

uint Inodes::create
    (FileVolume * pfv, uint nBegin, uint nInodes, uint iHeight) {
  uint iWidth = 4;		// 4 bytes, assumption

  // *INDENT-OFF* // Decide how many indirect entries to have 
  uint iIndirect = 0;
  if (iHeight > 3)    iIndirect = 1;
  if (iHeight > 4)    iIndirect = 2;
  if (iHeight > 5)    iIndirect = 3;
  // *INDENT-ON*

  fv = pfv;
  uint bsz = fv->superBlock.nBytesPerBlock;
  fv->superBlock.nBlockBeginInodes = nBegin;
  fv->superBlock.nInodes = nInodes;
  fv->superBlock.iWidth = iWidth;
  fv->superBlock.iHeight = iHeight;
  fv->superBlock.nBlocksOfInodes =
      (nInodes * iWidth * iHeight + bsz - 1) / bsz;
  fv->superBlock.inodesPerBlock = bsz / iWidth / iHeight;
  fv->superBlock.iDirect = iHeight - 1 - iIndirect;	// -1 for fileSize

  // set all inodes on disk to zero, and mark the inode blocks as not free
  uintbuffer = (uint *) new byte[bsz];
  memset(uintbuffer, 0, bsz);
  for (uint i = fv->superBlock.nBlockBeginInodes,
       j = i + fv->superBlock.nBlocksOfInodes; i < j; i++) {
    fv->writeBlock(i, uintbuffer);
    fv->fbvBlocks.setBit(i, 0);
  }
  return nInodes;
}

uint Inodes::reCreate(FileVolume * pfv)
{
  fv = pfv;
  uint bsz = fv->superBlock.nBytesPerBlock;
  uintbuffer = (uint *) new byte[bsz];
  return fv->superBlock.nInodes;
}

/* pre:: 0 < in < nInodes;; Note: We deliberately do not require
 * fbvInodes.getBit(in) == 1;; post:: Read the inode numbered in from
 * disk into uintbuffer, and return a ptr to it. If ne != 0, set it to
 * the number of blocks in file. */

uint *Inodes::getInode(uint in, uint * ne)
{
  uint nblock = fv->superBlock.nBlockBeginInodes
      + in / fv->superBlock.inodesPerBlock;
  fv->readBlock(nblock, uintbuffer);
  uint *pin = uintbuffer
      + (in % fv->superBlock.inodesPerBlock) * fv->superBlock.iHeight;
  if (ne != 0) {
    uint bsz = fv->superBlock.nBytesPerBlock;
    uint fileSize = pin[fv->superBlock.iHeight - 1];
    *ne = (fileSize + bsz - 1) / bsz;
  }
  return pin;
}

/* pre:: 0 < in < nInodes ;; post:: Write the inode numbered to
 * disk from uintbuffer. Other inodes that happen to be in the
 * uintbuffer also will be written out to disk. */

uint Inodes::putInode(uint in)
{
  uint nblock = fv->superBlock.nBlockBeginInodes
      + in / fv->superBlock.inodesPerBlock;
  return fv->writeBlock(nblock, uintbuffer);
}

/* pre:: inodes.created ;; post:: Return the number of a free inode,
 * if available, 0 otherwise. */

uint Inodes::getFreeInode()
{
  uint n = fv->fbvInodes.getFreeBit();
  // set the inode to all-zero
  memset(getInode(n, 0), 0,
	 fv->superBlock.iWidth * fv->superBlock.iHeight);
  return n;
}

/* pre:: 0 < in < nInodes ;; post:: Return the size of file whose
 * i-number is in. */

uint Inodes::getFileSize(uint in)
{
  uint *pin = getInode(in, 0);
  return pin[fv->superBlock.iHeight - 1];
}

uint Inodes::setFileSize(uint in, uint sz)
{
  uint *pin = getInode(in, 0);
  uint fileSize = pin[fv->superBlock.iHeight - 1] = sz;
  putInode(in);
  return fileSize;
}

uint Inodes::incFileSize(uint in, int inc)
{
  uint *pin = getInode(in, 0);
  pin[fv->superBlock.iHeight - 1] += inc;
  putInode(in);
  return pin[fv->superBlock.iHeight - 1];
}

uint Inodes::addTripleIndirect(uint * pin, uint nu, uint bn)
{
  return TODO("Inodes::addTripleIndirect");
}

uint Inodes::addDoubleIndirect(uint * pin, uint nu, uint bn)
{
  return TODO("Inodes::addDoubleIndirect");
}

uint Inodes::addSingleIndirect(uint * pin, uint nu, uint bn)
{
  return TODO("Inodes::addSingleIndirect");
}

/* pre:: none;; post:: To inode numbered in, append block number bn.  */

uint Inodes::addBlockNumber(uint in, uint bn)
{
  if (in == 0 || in >= fv->superBlock.nInodes
      || bn == 0 || bn > fv->superBlock.nTotalBlocks)
    return 0;

  uint bnspb = fv->superBlock.nBytesPerBlock / fv->superBlock.iWidth;
  uint iDirect = fv->superBlock.iDirect;
  uint iIndirectOne = iDirect + bnspb;
  uint iIndirectTwo = iIndirectOne + bnspb * bnspb;
  uint iIndirectThree = iIndirectTwo + bnspb * bnspb * bnspb;

  uint nu, *pin = getInode(in, &nu);

  // *INDENT-OFF*
  if (nu >= iIndirectThree)    return 0; // beyond capacity!
  else if (nu >= iIndirectTwo) nu = addTripleIndirect(pin, nu, bn);
  else if (nu >= iIndirectOne) nu = addDoubleIndirect(pin, nu, bn);
  else if (nu >= iDirect)      nu = addSingleIndirect(pin, nu, bn);
  // *INDENT-ON*
  else {
    pin[nu] = bn;		// nu < iDirect
    putInode(in);
  }
  return nu + 1;
}

/* pre:: in > 0;; post:: return the last block of inode numbered in. */

uint Inodes::lastBlock(uint in)
{
  uint nu, *pin = getInode(in, &nu);
  return (nu == 0 ? 0 : nu > fv->superBlock.iDirect ?
	  TODO("lastBlock") : pin[nu - 1]);
}

/* pre:: in > 0;; post:: Return the n-th block number of inode
 * numbered in. */

uint Inodes::getBlockNumber(uint in, uint nth)
{
  uint *pin = getInode(in, 0);
  return (nth >= fv->superBlock.iDirect ?
	  TODO("getBlockNumber") : pin[nth]);
}

/* pre:: getBit(in) == 1;; post:: Release the inode numbered in, and
 * the blocks referred to in it. ;; */

uint Inodes::releaseBlocks(uint in)
{
  uint nu, *pin = getInode(in, &nu), iDirect = fv->superBlock.iDirect;
  if (nu >= iDirect) {
    TODO("Inodes::release");
  }
  nu = fv->superBlock.iHeight - 1;
  for (;;) {
    pin[nu] = 0;
    if (nu == 0) break;
    uint bn = pin[--nu];
    if (bn > 0) fv->fbvBlocks.setBit(bn, 1);
  }
  putInode(in);
  return 1;
}

uint Inodes::release(uint in)
{
  releaseBlocks(in);
  fv->fbvInodes.setBit(in, 1);
  return 1;
}

/* pre:: none;; post:: Print out the contents of inode numbered in a
 * readble manner. */

uint Inodes::show(uint in)
{
  uint *pin = getInode(in, 0), x = 0;
  printf("inode #%d == [", in);
  for (; x < fv->superBlock.iHeight - 1; x++) {
    printf(" %d:%d", x, pin[x]);
  }
  printf(" size=%d]\n", pin[x]);
  return 1;
}

// -eof-
