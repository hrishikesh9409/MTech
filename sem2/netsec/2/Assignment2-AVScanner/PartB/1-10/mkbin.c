#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
    /* Create the file */
    int x, i = 1, y, z, count;

    printf("\nEnter the number of binary files to be created : \n");
    scanf("%d", &count);

    srand(time(NULL));
	int r = (rand() % count) + 1;

    char* name;
    char* bin = ".bin";
    FILE *fh;
    while(i <= count){
    	x = i;
    	y = rand();
    	sprintf(name, "%d", x);
    	strcat(name, bin);
    	fh = fopen (name, "wb");
    	if (fh != NULL) {
        	fwrite (&y, sizeof (y), 1, fh);
        	fclose (fh);
    	}
    	i++;
	}
	
/* Read the file back in */

//Debugging purposes :
/*  	i = 1;  
	while(i <= count){
    	x = i;
    	sprintf(name, "%d", x);
    	fh = fopen (name, "rb");
    	if (fh != NULL) {
        	fread (&z, sizeof (z), 1, fh);
        	fclose (fh);
    	}
    	printf ("Value is: %d\n", z);
    	i++;
	}*/
    return 0;
}