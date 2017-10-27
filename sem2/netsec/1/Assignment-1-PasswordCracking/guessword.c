/*
	BY 																		LAB	1
	Hrishikesh N 							Program to crack passwords encrypted by MD5 hashing using dictionary attack
	AM.EN.P2CSN16008
*/


#define _XOPEN_SOURCE

#include <unistd.h>						//using the crypt function
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>						// using standard string operations 

int main(int argc, char* argv[]){		// parameters passed via commandline arguments 

	int option;							//this variable is used to by getopt method to parse the respective parameters 

	FILE *file1, *file2, *file3;		// open file pointers 
	char* p;							// 3 char* pointers to store the contents of optarg
	char* q;
	char* r;

	char c[512], d[512];				/* declare a char array to store contents while reading from a file*/

	char* epasswd;						//Encrypted password as found in the passwordfile.txt
	char* a;							//buffer storing contents when read through the file passwordfile.txt
	char* username;						//stores username as read from the passwordfile.txt
	char* salt;							//stores salt value as read from the file

	char* b;							//buffer storing contents when read through the file dictionary.txt
	char* plainPasswd;					//stores plainpassword after tokenization 

	char* result;						//final value used to compare with the encrypted passwords to find matches 
	char* concat;						//concatenate values of both salt and the algorithm 
	char* output;						//output value from the crypt function 
	char* algorithm = "$1$";			//default encryption algorithm value used is MD5 

	/*			Error checking for passing parameters		*/
	if(argc == 1){
		printf("\nRequires 3 parameters\n\n");
		exit(-1);
	}
	else if(strcmp(argv[1], "-i") != 0 && (strcmp(argv[1], "-d")) != 0 && (strcmp(argv[1], "-o")) != 0){
		printf("\nInput format improper: Need usage of -i or -d or -o before argument values\n\n");
		exit(-1);
	}
	else if(argc == 2 || argc == 3){
		if(argc == 2){
			printf("\n%s: option requires an argument -- '%s'", argv[0], argv[1]);
			printf("\nRequires 3 parameters for program to run\n\n");			
		}
		if(argc == 3){
			printf("\nRequires 3 parameters for program to run\n\n");
		}
		exit(-1);
	}
	else if(argc == 4 || argc == 5){
		if(argc == 4){
			printf("\n%s: option requires an argument -- '%s'", argv[0], argv[3]);
			printf("\nRequires 3 parameters for program to run\n\n");
		}
		if(argc == 5)
			printf("\nRequires 3 parameters for program to run\n\n");
		exit(-1);
	}
	else{			/*	if all parameters are passed correct then parse one by one 	*/
		while((option = getopt(argc, argv, "i:d:o:")) != -1){
			if(option == 'i'){
				p = optarg;
			}	
			else if(option == 'd'){
				q = optarg;
			}
			else if(option == 'o'){
				r = optarg;
			}
		}
	}
	/*		Program to decrypt and compare passwords begins here 	*/
	if(p != NULL && q != NULL && r != NULL){

		file1 = fopen(p, "r");			//Open files 
		file2 = fopen(q, "r");
		file3 = fopen("allcrackedpasswords.txt","w");

		if(strcmp(r, "all") == 0){					//check if the last passed parameter is all or simply a username
			if(fgets(c, sizeof(c), file1) != NULL){
				username = strtok(c, ":");			//get username
				a = strtok(NULL, "$");
				salt = strtok(NULL, "$");			//get salt value
			}
		}
		else{										//if specific username is given find the corresponding encrypted password
			while(fgets(c, sizeof(c), file1) != NULL){
				username = strtok(c, ":");			
				a = strtok(NULL, "$");	
				salt = strtok(NULL, "$");
				epasswd = strtok(NULL, ":");

				if(strcmp(r, username) == 0)
					break;
				else 
					continue;
			}
		}
		while (fgets(d, sizeof(d), file2) != NULL){			//use dictionary file to match and compare passwords 
			b = strtok(d, "\t");
			b = strtok(NULL, "\t");
			b = strtok(NULL, "\t");
			plainPasswd = strtok(NULL, "\n");				//read and store plainpassword values 

			concat = malloc(strlen(algorithm) + strlen(salt));		//conctenate algorithm and salt values
			strcpy(concat, algorithm);
			strcat(concat, salt);
			//printf("%s\n", concat);						used for debugging purpose

			output = malloc(strlen(plainPasswd) + strlen(concat));	
			output = crypt(plainPasswd, concat);			//output stores the value after passing through the crypt function for encryption

			result = strtok(output, "$");
			result = strtok(NULL, "$");
			result = strtok(NULL, "\n");					//parsing out and storing the required textual encrypted part of the output value

			if(strcmp(r, "all") == 0){						//if -o all was passed at cli then exhaustively search through every single value 
				file1 = fopen(p, "r");
				
				while(fgets(c, sizeof(c), file1) != NULL){
					username = strtok(c, ":");
					a = strtok(NULL, "$");
					salt = strtok(NULL, "$");
					epasswd = strtok(NULL, ":");
					if(strcmp(result, epasswd) == 0){
					//printf("%d. %s : %s = %s\n", i, username, plainPasswd, epasswd);		used for debugging purpose
						fprintf(file3,"%s:%s\n", username, plainPasswd);	//write the results back to allcrackedpasswords.txt
					}
				}
			}
			else{											//if -o username was passed at cli compare one value of the encrypted password against all other encrypted passwords 
				if(strcmp(result, epasswd) == 0){
					//printf("%s = %s\n", username, plainPasswd);							used for debugging purpose 
					fprintf(file3,"%s:%s\n", username, plainPasswd);		//write the results back to allcrackedpasswords.txt
				}
			}
			free(concat);									// free dynamically allocated memory 
		}
	}
	return 0;
}
