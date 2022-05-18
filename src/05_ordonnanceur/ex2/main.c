
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define NBR_BLOCK 50
#define MEBIBYTE  1048576

int main(int argc, char const *argv[])
{
    (void) argc;
    (void) argv;
    int i = NBR_BLOCK;
    int finished = 1;
    while(i--){
        void* useless_things = malloc(MEBIBYTE);
        if(useless_things == NULL){
            printf("I so sad to not have enought memory :'(\n");
            finished = 0;
            break;

        }else{
            printf("Yaaiiiiiix I have one more bloc ! (%d / %d)\n", NBR_BLOCK - i, NBR_BLOCK);
            memset(useless_things, 0, MEBIBYTE);
        }
        sleep(1);
    }
    if(!finished){
        printf("Giving this this job up :'(\n");
    }else{
        printf("Ended successfully\n");
    }

    return 0;
}
