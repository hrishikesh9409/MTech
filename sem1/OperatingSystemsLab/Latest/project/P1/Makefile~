#  Makefile for P0: File Sys Project Intro;  CEG 4350/6350 P. Mateti

CFLAGS = -g -Wall -ansi -pedantic 
CC = g++

.SUFFIXES: .cpp .o .C

.C.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<


FILES= ReadMe.txt directory.C file.C iNodes.C simDisk.C Makefile	\
	bitVector.C diskParams.dat fstypes.h shell.C volume.C

OBJFILES = simDisk.o bitVector.o directory.o file.o	\
	iNodes.o volume.o shell.o

PROJECT = P0

$(PROJECT): $(OBJFILES)
	g++ -o $(PROJECT) $(CFLAGS) $(OBJFILES)

test:   $(PROJECT)	
	rm -fr D?.bin
	./$(PROJECT)

$(OBJFILES): fstypes.h Makefile

indent:
	indent -kr -i2 -pmt *.C *.h

tar archive: clean
	(cd ..; tar cvvfj ./$(PROJECT).tbz $(PROJECT); ls -l $(PROJECT).tbz)

clean:
	rm -fr *.o *~ *.out $(PROJECT) D?.dsk *.f33 d1.txt


# -eof-
