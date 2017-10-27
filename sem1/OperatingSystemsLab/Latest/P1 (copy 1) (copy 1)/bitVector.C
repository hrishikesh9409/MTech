/*
 * bitVector.C
 * prabhaker.mateti@wright.edu
 */

#include "fstypes.h"

/* pre:: nbits > 0;; post:: Construct fbv[], and initialize it to all
 * 1s. Also, write the bit vector to disk. Return the number of blocks
 * occupied. */

uint BitVector::create(FileVolume * pfv, uint nbits, uint nblockbegin)
{
  fv = pfv;
  uint bsz = fv->superBlock.nBytesPerBlock;
  bitVector = new byte[bsz];

  nBits = nbits;
  nBlockBegin = nblockbegin;

  // write the bit-vector to disk
  uint nbytes = (nbits + 7) / 8;	// number of bytes
  uint nBlocksLong = (nbytes + bsz - 1) / bsz;
  memset(bitVector, 0xFF, bsz);	// set all bits to free, i.e., 1
  for (uint i = 0; i < nBlocksLong; i++)
    fv->writeBlock(nBlockBegin + i, bitVector);

  // mark the blocks used by the bit-vector as not free
  for (uint bn = nBlockBegin + nBlocksLong - 1; bn >= nBlockBegin; bn--)
    fv->fbvBlocks.setBit(bn, 0);
  return nBlocksLong;
}

uint BitVector::reCreate(FileVolume * pfv, uint nbits, uint nblockbegin)
{
  fv = pfv;
  uint bsz = fv->superBlock.nBytesPerBlock;
  bitVector = new byte[bsz];
  if (bitVector == 0)
    return 0;			// could not get mem for bitVector

  nBits = nbits;
  nBlockBegin = nblockbegin;
  uint nbytes = (nbits + 7) / 8; // number of bytes
  return (nbytes + bsz - 1) / bsz;
}

/* pre:: none;; post:: return freeBit[x]. */

uint BitVector::getBit(uint x)
{
  if (x > nBits)
    return 0;			// illegal value

  uint bsz = fv->superBlock.nBytesPerBlock;
  uint xbyte = x / 8;		// bit index converted to byte index
  uint xblock = xbyte / bsz;	// converted to block index
  fv->readBlock(nBlockBegin + xblock, bitVector);

  uint b = bitVector[xbyte % bsz];	// our needed bit is in b
  uint f = x % 8;		// bit f of b is wanted
  b >>= 7 - f;			// right shift b by (7-f) positions
  return b & 1;			// return the last bit
}


/* pre:: v==0, or 1;; post:: freeBit[x] := v, and write it to the
 * disk;; */

void BitVector::setBit(uint x, uint v)
{
  if (x > nBits)
    return;			// illegal value

  uint bsz = fv->superBlock.nBytesPerBlock;
  uint xbyte = x / 8;		// bit index converted to byte index
  uint xblock = xbyte / bsz;	// converted to block index
  fv->readBlock(nBlockBegin + xblock, bitVector);

  uint f = x % 8;
  uint m = 1 << (7 - f);	// 00...010...0, bit 1 in the f-th position
  uint b = bitVector[xbyte % bsz];
  bitVector[xbyte % bsz] = (v != 0 ? b | m : b & ~m);
  fv->writeBlock(nBlockBegin + xblock, bitVector);
}

/* pre:: none ;; post:: Return the number i > 0 of a free bit, if
 * available i.e., freeBit[i] == 1 and set the bit to 0, 0
 * otherwise. */

uint BitVector::getFreeBit()
{
  for (uint i = 1; i < nBits; i++)
    if (getBit(i) == 1) {
      setBit(i, 0);
      return i;
    }
  return 0;
}

// -eof-
