/*
 * directory.C of CEG 433/633 File Sys Project; This is extremely
 * incomplete.

 * prabhaker.mateti@wright.edu
 */

#include "fstypes.h"

/* pre:: pfv mostly constructed;; post:: The root dir is created. */

uint Directory::create(FileVolume * pfv)
{
  fv = pfv;
  nInode = 1;
  fv->fbvInodes.setBit(1, 0);	// inode #1 no longer free
  char dirEntry[] = ".\00001..\00001"; // suspicious!

  dirf = new File(fv, nInode);
  dirf->appendBytes(dirEntry, 2 + 4 + 3 + 4);
  delete dirf;
  return 1;
}

uint Directory::reCreate(FileVolume * pfv)
{
  fv = pfv;
  return nInode = 1;
}

// -eof-

