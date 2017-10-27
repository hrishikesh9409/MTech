#include <stdio.h>
#include <string.h>
int bof(char *str)
{
 char buffer[80];
 getchar();
 /* The following statement has a buffer overflow problem */
 strcpy(buffer, str);
 return 1;
}
int main(int argc, char **argv)
{
 char str[100];
 FILE *badfile;
 badfile = fopen("badfile", "r");
 fread(str, sizeof(char), 517, badfile);
 bof(str);
 printf("Returned Properly\n");
 return 1;
} 