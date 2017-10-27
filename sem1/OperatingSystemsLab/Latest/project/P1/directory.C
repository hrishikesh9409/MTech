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

uint Directory::createFile(char *nm, FileVolume * pfv){
  fv = pfv; 
  uint free_inode = fv->inodes.getFreeInode();
  fv->fbvInodes.setBit(free_inode, 0);
  char buffer[50];
  sprintf(buffer,"%d %s",free_inode, nm);
  //printf("%s\n", buffer); 
  char dirEntry[] = ".\00001..\00001";
/*  char v[50];
  v = char(dirEntry);
  dirf = new File(fv, free_inode);
  dirf->appendBytes(buffer, 2 + 4 + 3 + 4);
  //strcat(dirEntry, buffer);
  printf("\n\n%s\n", dirEntry);*/
  return 1;
}

// -eof-

