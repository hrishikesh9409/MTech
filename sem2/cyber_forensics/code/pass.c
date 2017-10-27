#include <stdio.h>
#include <strings.h>

int main(int argc, char **argv){

	char password[12];
	printf("Enter your password: ");
	gets(password);

	if(strcmp("root", password) == 0){
		printf("U are now root user!\n");
	}
	else{
		printf("Password incorrect!\n");
	}

	return 0;
}