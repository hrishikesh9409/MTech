#include <stdio.h>
#include <string.h>

void vuln_fun(char * name)
{
  char buf[32];
  printf("Address of buf is at %p\n", buf );
  strcpy(buf, name);
}


int main(int argc, char *argv[])
{
  vuln_fun(argv[1]);
  return 0;
}
