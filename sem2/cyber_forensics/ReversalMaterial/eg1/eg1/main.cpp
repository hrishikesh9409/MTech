#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define password "root"

int main(int argc, char *argv[])
{
  char pass[100];


  printf("Please enter the password\n\n");
  scanf("%s", pass);
  if ( strcmp(pass, password) == 0 )
  {
       printf("Congratulations!!! You are now Root User!!\n\n");
  }
  else
  {
      printf("Wrong Password! Authentication Failure!!\n\n");
  }

  system("PAUSE");
  return 0;
}
