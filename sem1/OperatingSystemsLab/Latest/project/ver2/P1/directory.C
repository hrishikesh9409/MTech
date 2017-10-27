/*
 * directory.C of CEG 433/633 File Sys Project; This is extremely
 * incomplete.

 * prabhaker.mateti@wright.edu
 */

#include "fstypes.h"

/* pre:: pfv mostly constructed;; post:: The root dir is created. */

/*	struct content{
		int array[20];
	};*/


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

  sprintf(buffer,"%d%s",free_inode, nm);

  int len = strlen(nm);

  dirf = new File(fv, free_inode);
  dirf->appendBytes(buffer, len + 1 + 2);


/*  strncpy(copy, dirEntry, sizeof(copy));
  copy[sizeof(copy) - 1] = '\0';
  sprintf(buffer,"%s %d",nm, free_inode);
  strcat(copy, buffer);

  printf("\n%s\n", copy);
  dirf = new File(fv, free_inode);
  dirf->appendBytes(copy, 2 + 4 + 3 + 4);*/

  return 1;
}

/*Logic is to print parse out the readable characters from the hexdump store it in an array of some sort and then format and print it to the screen. Its half implemented and the rest half is to create a new array to store the readable ascii values, convert them to text and then format and display them*/

uint Directory::ls(FileVolume * pfv, uint no_blocks, uint Bytes, uint mf){
	char * bp = (char*)calloc(sizeof(char), 512);
	int n;
	for(uint i = 4; i < 10; i++){
		n = fv->readBlock(i, bp);
		for(uint j = 0; j < Bytes; j++){
			if(bp[j] > 0){
				printf("%x", bp[j]);
				//c[i].array[j] = bp[j];
			}
		}
		printf("\n");
	}
	

	printf("\n%d\n", n);
/*
	for(int i = 4; i < 7; i++){
		for(int j = 0; j < 10; j++){
			printf("%d", c[i].array[j]);
		}
		printf("\n");
	}
*/	return 1;
}


uint Directory::rm(char *nm, FileVolume * pfv){

	return 1;
}

// -eof-

