/*
                                                                AV Scanner Program

    By 
    Hrishikesh N
    AM.EN.P2CSN16008
    S2 - MTech Cyber Security Systems and Networks
*/



#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

int j = 0;
int k = 0;
int counter = 0;

char buffer[4096];

char *path[1024];
char *hash[1024];
char temp1[1024];
char temp2[1024];

char *filepath[1024];
char *infected_hash[1024];
char temporary1[1024];
char temporary2[1024];

int main(void)
{
    DIR *d;
    char *ext = ".bin";
    struct dirent *dir;
    char sig[] = "DEADBEEFDEAD";

    d = opendir(".");
    printf("\n\nOriginal Files and their Hash Values\n\n");
    if(d){
        while((dir = readdir(d)) != NULL){
            if(strstr(dir->d_name, ext) !=  0){
                path[j] = malloc(100 * sizeof(char));
                hash[j] = malloc(100 * sizeof(char));
                unsigned char c[MD5_DIGEST_LENGTH];
                strcpy(path[j], dir->d_name);
                char *filename = path[j];
                int i;
                FILE *inFile = fopen (filename, "rb");
                MD5_CTX mdContext;
                int bytes;
                unsigned char data[1024];

                if (inFile == NULL) {
                    printf ("%s can't be opened.\n", filename);
                    return 0;
                }

                MD5_Init (&mdContext);
                while ((bytes = fread (data, 1, 1024, inFile)) != 0)
                    MD5_Update (&mdContext, data, bytes);
                MD5_Final (c,&mdContext);
                for(i = 0; i < MD5_DIGEST_LENGTH; i++){
                    //printf("%02x", c[i]);
                    sprintf(temp1, "%02x", c[i]);
                    strcat(temp2, temp1);
                }
                strcpy(hash[j], temp2);
                printf ("%s : %s\n", path[j], hash[j]);
                fclose (inFile);

                j++;
                strcpy(temp2 ,"");
            }
        }
    }
    k = j;
    printf("\n\n");


    while(1){
        j = 0;
        sleep(2);
        d = opendir(".");
        if(d){
            while((dir = readdir(d)) != NULL){
                if(strstr(dir->d_name, ext) !=  0){
                    filepath[j] = malloc(100 * sizeof(char));
                    infected_hash[j] = malloc(100 * sizeof(char));
                    unsigned char c[MD5_DIGEST_LENGTH];
                    strcpy(filepath[j], dir->d_name);
                    char *filename = filepath[j];
                    int i;
                    FILE *inFile = fopen (filename, "rb");
                    MD5_CTX mdContext;
                    int bytes;
                    unsigned char data[1024];

                    if (inFile == NULL) {
                        printf ("%s can't be opened.\n", filename);
                        return 0;
                    }

                    MD5_Init (&mdContext);
                    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
                        MD5_Update (&mdContext, data, bytes);
                    MD5_Final (c,&mdContext);
                    for(i = 0; i < MD5_DIGEST_LENGTH; i++){
                    //printf("%02x", c[i]);
                        sprintf(temporary1, "%02x", c[i]);
                        strcat(temporary2, temporary1);
                    }
                    strcpy(infected_hash[j], temporary2);
                    if(strcmp(hash[j], infected_hash[j]) != 0){
                        //printf ("%s : %s\n", filepath[j], infected_hash[j]);
                        FILE *f = fopen (filepath[j], "rb");
                        if(f){
                            fseek (f, 0, SEEK_END);
                            fseek (f, 0, SEEK_SET);
                            if (buffer)
                            {
                                fread (buffer, 1, 4096, f);
                            }
                            fclose (f);
                        }
                        if(strstr(buffer, sig) == 0){
                            if(counter < k){
                                printf ("%s : %s    VIRUS INFECTED!\n", filepath[j], infected_hash[j]);
                                counter++;
                            }
                            if(counter == k){
                                printf("Enter q to quit the program\n");
                                counter++;
                                char quit;
                                label:
                                scanf("%c", &quit);
                                if(quit == 'q')
                                    exit(1);
                                else{
                                    goto label;
                                }
                            }
                        }
                    }
                    fclose (inFile);
                    j++;
                    strcpy(temporary2 ,"");
                }
            }
        }
    }
    return 0;
}