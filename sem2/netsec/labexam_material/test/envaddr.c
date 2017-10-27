#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	char* shell = getenv("EGG");
	printf("%x\n", shell);
	return 0;
}