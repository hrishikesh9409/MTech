/*
	BY 																		LAB	1
	Hrishikesh N 									Program to unshadow given password file and a shadow file
	AM.EN.P2CSN16008
*/

#include <stdio.h>
#include <string.h>   //standard string operations 

int main(int argc, char* argv[])	//passing parameters through commandline arguments
{
  char c[256];  /* declare a char array to store contents while reading from a file*/
  char d[256];	/* declare a char array */
  FILE *file,*filep,*file2;  /* declare a FILE pointers */
  file = fopen(argv[2], "r");	//First file
  file2 = fopen(argv[1], "r");	//Second file
  filep = fopen("passwordfile.txt","w+"); //Output file
  char *p;	/*	2 char* pointers p and q to read from file 	*/
  char *q;	

  while(fgets(c, sizeof(c), file2) != NULL && fgets(d, sizeof(d), file) != NULL){
      q = strtok(d, ":");	//Using strtok fn to tokenize from file
      fprintf(filep,"%s:",q);	//Write token to  output file
      //printf("%s:", q);
      q = strtok(NULL, ":");
      //printf("%s:", q);
      fprintf(filep,"%s:",q);
      

      p = strtok(c, ":");
      p = strtok(NULL, ":");
      p = strtok(NULL, ":");
      //printf("%s:", p);
      fprintf(filep,"%s:",p);
      p = strtok(NULL, ":");
      //printf("%s:", p);
      fprintf(filep,"%s:",p);
      p = strtok(NULL, ":");
      //printf("%s:", p);
      fprintf(filep,"%s:",p);
      p = strtok(NULL, ":");
      //printf("%s:", p);
      fprintf(filep,"%s:",p);
      p = strtok(NULL, ":");
      //printf("%s", p);
      fprintf(filep,"%s",p);
      //printf("\n");
  }
  fclose(file);		//close file and free buffers i.e. file pointers
  fclose(file2);
  fclose(filep);
  return 0;
}