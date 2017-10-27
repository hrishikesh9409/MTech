
/*
 * volume.C of CEG4350 OS File Sys Project; This code is "kind of"
 * complete, but may look puzzling because other parts (e.g.,
 * directories) are incomplete.  Assumes that one block equals one
 * sector.

 * prabhaker.mateti@wright.edu
 */

#include "fstypes.h"

/* pre:: psimDisk != 0;; post:: On psimDisk, construct a new file
 * volume with nInodes and of htInode. */

FileVolume::FileVolume(SimDisk * psimDisk, uint nInodes, uint htInode)
{
  simDisk = psimDisk;
  if (nInodes == 0 || htInode == 0)
    return;

  superBlock.nBytesPerBlock = simDisk->nBytesPerSector;	// for now
  superBlock.nTotalBlocks = simDisk->nSectorsPerDisk;

  superBlock.nBlocksFbvBlocks =
      fbvBlocks.create(this, superBlock.nTotalBlocks, 1);
  superBlock.nBlocksFbvInodes =
      fbvInodes.create(this, nInodes, 1 + superBlock.nBlocksFbvBlocks);

  inodes.create
      (this, 1 + superBlock.nBlocksFbvBlocks + superBlock.nBlocksFbvInodes,
       nInodes, htInode);

  superBlock.nBlockBeginFiles =
      superBlock.nBlockBeginInodes + superBlock.nBlocksOfInodes;

  superBlock.fileNameLengthMax = simDisk->nBytesPerSector;	// for now
  root.create(this);

  byte *bp = new byte[superBlock.nBytesPerBlock];
  memcpy(bp, &superBlock, sizeof(superBlock));
  writeBlock(0, bp);		// write it as block# 0
  delete bp;
}

/* pre:: On psimDisk, there already is a file volume properly made;;
 * post:: Re-create the file volume object;; */

FileVolume::FileVolume(SimDisk * psimDisk)
{
  simDisk = psimDisk;
  byte *bp = new byte[simDisk->nBytesPerSector];

  // set the superBlock from block 0 of disk
  readBlock(0, bp);
  memcpy(&superBlock, bp, sizeof(superBlock));
  delete bp;

  fbvBlocks.reCreate(this, superBlock.nTotalBlocks, 1);
  fbvInodes.reCreate(this, superBlock.nInodes,
		     1 + superBlock.nBlocksFbvBlocks);
  inodes.reCreate(this);
  root.reCreate(this);
}

uint FileVolume::writeBlock(uint nBlock, void *p)
{
  return simDisk->writeSector(nBlock, p);
}

uint FileVolume::readBlock(uint nBlock, void *p)
{
  return simDisk->readSector(nBlock, p);
}

// -eof-
