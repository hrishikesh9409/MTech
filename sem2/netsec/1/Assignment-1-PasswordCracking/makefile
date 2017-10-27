CC=gcc
all:
	gcc unshadow.c -o unshadow
	gcc guessword.c -lcrypt -o guessword
runall:unshadow guessword
	./unshadow password.txt shadow.txt
#	./guessword -i passwordfile.txt -d dictionary.txt -o username : eg. dmr362
	./guessword -i passwordfile.txt -d dictionary.txt -o all
