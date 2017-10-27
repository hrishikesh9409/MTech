#include <stdio.h>
#include <string.h>

void vuln_fun()
{
  char buf[32];
  printf("Address of buf is at %p\n", buf );
  gets(buf);
}


int main(int argc, char *argv[])
{
  vuln_fun();
  return 0;
}
