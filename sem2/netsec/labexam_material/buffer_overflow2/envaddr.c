#include <stdio.h>
#include <stdlib.h>

void main()
{ 
  char* shell = getenv("EGG");

  if (shell)
  printf("%x\n", shell);

}
