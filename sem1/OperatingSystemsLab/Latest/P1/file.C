/* 
 * file.C of CEG 4350 File Sys Project
 * prabhaker.mateti@wright.edu
 */

#include "fstypes.h"

/* pre:: pfv != 0;; post:: in > 0 => "Open" an FS33 file for
 * read-and-write whose i-node is numbered in; in == 0 => Create a new
 * file with size == 0;; */

File::File(FileVolume * pfv, uint in)
{
  fv = pfv;
  buffer = new byte[fv->superBlock.nBytesPerBlock];
  nInode = (in > 0 ? in : fv->inodes.getFreeInode());
}

File::~File()
{
  delete buffer;
}

/* pre:: ;; post:: Deposit into bp[] the content of
 * nx-th block of file. */

uint File::readBlock(uint nx, void *bp)
{
  uint bn = fv->inodes.getBlockNumber(nInode, nx);
  if (bn == 0)    
    return nBytesInThisBlock = 0;

  fv->readBlock(bn, bp);

  uint bsz = fv->superBlock.nBytesPerBlock;
  uint nPrecedingBytes = (nx > 0 ? nx * bsz : 0);
  uint nBytesAvailable = fv->inodes.getFileSize(nInode) - nPrecedingBytes;
  nBytesInThisBlock = (nBytesAvailable > bsz? bsz : nBytesAvailable);
  return nBytesInThisBlock;
}

/* pre:: none;; post:: Make preparations so that we can read from
 * "this" file byte by byte. */

void File::prepareToReadByteByByte()
{
  readBlock(0, buffer);		// read block# 0 of this file
  xNextByte = 0;		// index of next byte that caller will get
  nBlocksReadSoFar = 1;
}

/* pre:: prepareReadByteByByte() has been called;; post:: If this
 * file reading has not exhausted all its content, deposit into bp[0]
 * the next byte. Return 0 for failure, >0 for success. */

uint File::nextByte(byte * bp)
{
  if (xNextByte == nBytesInThisBlock) {
    if (readBlock(nBlocksReadSoFar++, buffer) == 0)
      return 0;
    xNextByte = 0;
  }
  *bp = buffer[xNextByte++];
  return 1;
}

/* pre:: p != 0, 0 <= iz < nBytesPerBlock ;; post:: Append the p[0
 * .. iz-1] as a block at the end of this file.  Return the number of
 * bytes so written. */

uint File::appendOneBlock(void *p, uint iz)
{
  memcpy(buffer, p, iz);
  uint bn = fv->fbvBlocks.getFreeBit();
  fv->inodes.addBlockNumber(nInode, bn);
  fv->writeBlock(bn, buffer);
  fv->inodes.incFileSize(nInode, iz);
  return iz;
}

uint File::writeBlock(uint nBlock, void *p)
{
  uint bn = fv->inodes.getBlockNumber(nInode, nBlock);
  if (bn == 0) return 0;

  return fv->writeBlock(bn, p);
}

/* pre:: none ;; post:: To this file, at the end of its current content,
 * append newContent[0..nBytes-1].  nBytes can be larger than bsz. */

uint File::appendBytes(void *newContent, uint nBytes)
{
  if (newContent == 0 || nBytes == 0)
    return 0;

  uint nWritten = 0, bsz = fv->superBlock.nBytesPerBlock, nb = 0;
  byte *newContentBp = (byte *) newContent;

  // Is the last block full?
  uint fileSize = fv->inodes.getFileSize(nInode);
  uint nFragmentSize = fileSize % bsz;
  if (nFragmentSize > 0) {
    uint nBlocks = (fileSize + bsz - 1) / bsz;
    readBlock(nBlocks - 1, buffer);
    nb = (nBytes > bsz - nFragmentSize ? bsz - nFragmentSize : nBytes);
    memcpy(buffer + nFragmentSize, newContentBp, nb);
    writeBlock(nBlocks - 1, buffer);
    fv->inodes.incFileSize(nInode, nb);
  }
  for (;;) {
    nWritten += nb;
    newContentBp += nb;
    nBytes -= nb;
    if (nBytes == 0)
      break;			// out of loop
    nb = (nBytes > bsz ? bsz : nBytes);
    appendOneBlock(newContentBp, nb);
  }
  return nWritten;
}

uint File::deletePrecedingBytes(uint howManyBytes)
{
  uint bsz = fv->superBlock.nBytesPerBlock;
  uint blockNumSource = nBlocksReadSoFar - 1;
  uint byteSource = xNextByte;
  uint blockNumDest = blockNumSource - 1;
  uint byteDest = bsz - howManyBytes + xNextByte;
  if (xNextByte >= howManyBytes) {
    blockNumDest++;
    byteDest = xNextByte - howManyBytes;
  }
  byte  *blockPtrDest = new byte[bsz], *blockPtrSource = buffer;
  readBlock(blockNumDest, blockPtrDest);
  uint nBytesToMove =  fv->inodes.getFileSize(nInode)
    - bsz * blockNumSource - xNextByte;
  while (nBytesToMove-- > 0) {
    blockPtrDest[byteDest++] = blockPtrSource[byteSource++];
    if (byteDest == bsz) {
      writeBlock(blockNumDest++, blockPtrDest);
      byteDest = 0;
    }
    if (byteSource == bsz) {
      uint nr = readBlock(++blockNumSource, blockPtrSource);
      byteSource = 0;
      if (nr == 0) break;
    }
  }
  if (byteDest < bsz) writeBlock(blockNumDest, blockPtrDest);
  fv->inodes.incFileSize(nInode, - howManyBytes);
  return 0;
}

// -eof-
