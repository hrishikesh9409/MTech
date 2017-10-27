/*
 * fstypes.h of CEG [46]350 File Sys Project
 * prabhaker.mateti@wright.edu
 */

#include <stdio.h>		// not all these are needed every where,
#include <string.h>		// but we take the simple way out
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

typedef unsigned char byte;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef unsigned long int ulong;
extern unsigned int maxF,n_inodes,inodeh;   // Declaring globally the variables for maxFnm nInodes iNodeHt



enum { LabelSZ = 15, SectorsMAX = 1024, BytesPerSectorMAX = 4096 };

struct DiskInfo {

  char labelOfTheDisk[LabelSZ + 1];
  uint nSectorsPerDisk;
  uint nBytesPerSector;

};

uint TODO();
uint TODO(const char *p);

class FileVolume;		// forward declaration

class SimDisk {
public:
  DiskInfo dI;

  SimDisk(const char *name, uint nspd, uint nbps);
  uint writeSector(uint nSector, void *p);
  uint readSector(uint nSector, void *p);

  FileVolume *makefs(uint nInodes, uint szInode);
  FileVolume *findfs();

private:
  uint paramsOK();
  int makeDiskImage();
  int openDiskImage(uint mode);
};

class SuperBlock {
public:
  uint magicNumber;		// == 9876543210
  uint labelSize;		// == LabelSZ
  //uint nSectorsPerDisk;
  uint nTotalBlocks;
  //uint nBytesPerSector;
  uint nBytesPerBlock;		// a multiple of nBytesPerSector

  uint nBlocksFbvBlocks;	// #blocks occupied by fbv of blocks
  uint nBlocksFbvInodes;	// #blocks occupied by fbv of inodes

  uint nBlockBeginInodes;	// where do the i-nodes begin?
  uint nBlocksOfInodes;		// #blocks on disk occupied by i-nodes
  uint inodesPerBlock;		// one block will contain this many inodes
  uint nInodes;			// total number of inodes
  uint iWidth;			// inode size on disk == iWidth * iHeight
  uint iHeight;			// iHeight == iDirect + iIndirect + 1
  uint iDirect;			// 0 <= iIndirect <= 3

  uint nBlockBeginFiles;	// == nBlockBeginInodes + nBlocksInode  
  uint fileNameLengthMax;

  char labelOfTheDisk[LabelSZ + 1];
};

class BitVector {
public:
  uint create(FileVolume * fv, uint nBits, uint nBeginBlock);
  uint reCreate(FileVolume * fv, uint nBits, uint nBeginBlock);
  uint getBit(uint indexOfBit);
  void setBit(uint indexOfBit, uint newValue);
  uint getFreeBit();
private:
  uint nBits;			// #bits in this vector
  uint nBlockBegin;		// at what block does the vector begin?
  byte *bitVector;		// ptr to one block-long area of mem
  FileVolume *fv;
};

class Inodes {
public:
  uint create(FileVolume * fv, uint nBegin, uint nInodes, uint htInode);
  uint reCreate(FileVolume * fv);	// in == i-node number
  uint *getInode(uint in, uint * ne);	// return ptr to inode in
  uint putInode(uint in);
  uint releaseBlocks(uint in);
  uint getFreeInode();
  uint release(uint in);

  uint lastBlock(uint in);
  uint getBlockNumber(uint in, uint nth);
  uint addTripleIndirect(uint * pin, uint nu, uint bn);
  uint addDoubleIndirect(uint * pin, uint nu, uint bn);
  uint addSingleIndirect(uint * pin, uint nu, uint bn);
  uint addBlockNumber(uint in, uint bn);

  uint getFileSize(uint in);
  uint setFileSize(uint in, uint sz);
  uint incFileSize(uint in, int increment);

  uint show(uint in);
private:
  uint * uintbuffer;		// inodes from one block
  FileVolume *fv;
};

class File {
public:
  uint nInode;			// inode number of this file
  byte *buffer;			// one blocksize buffer

  File(FileVolume * fv, uint nInode);
  ~File();

  uint readBlock(uint xthBlock, void *p);
  uint writeBlock(uint xthBlock, void *p);
  void prepareToReadByteByByte();
  uint nextByte(byte * bp);
  uint appendOneBlock(void *p, uint iz);
  uint appendBytes(void *newContent, uint nBytes);
  uint deletePrecedingBytes(uint howManyBytes);
private:
  uint nBlocksReadSoFar;
  uint nBytesInThisBlock;
  uint xNextByte;
  FileVolume *fv;
};

class Directory {
public:
  uint nInode;			// inode number of this directory

  uint create(FileVolume * fv);
  uint reCreate(FileVolume * fv);
  uint findName(char *nm);
  uint addName(char *nm, uint in);
  uint deleteName(char *nm, uint in);
  uint createFile(char *nm);
  uint ls();
  uint rm(char *nm);
private:
  File *dirf;			// this dir viewed as a normal file
  byte *dirEntry;		// area of mem for file name + i-num
  FileVolume *fv;
  char *getNextFileName();
  void closeDir();
  uint addNamePrivate(char *newName, uint in);
};

class FileVolume {
public:
  SimDisk * simDisk;
  SuperBlock superBlock;	// not a good choice, but ...
  BitVector fbvBlocks;		// not a good choice, but ...
  BitVector fbvInodes;		// not a good choice, but ...
  Inodes inodes;		// not a good choice, but ...
  Directory root;		// not a good choice, but ...

  FileVolume(SimDisk * simDisk, uint nInodes, uint szInode);
  FileVolume(SimDisk * simDisk);
  uint readfile(char *fsname, char *unixFilePath);
  uint writefile(char *fsname, char *unixFilePath);
  uint deletefile(char *fsname);

  uint writeBlock(uint nBlock, void *p);
  uint readBlock(uint nBlock, void *p);
};

// -eof-
