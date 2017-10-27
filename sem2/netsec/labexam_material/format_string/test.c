#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char b[128];
	strcpy(b, argv[1]);
	printf(b);
	printf("\n");

	return 0;
}