#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

int
main(int argc, char *argv[])
{
    char *my_env = "SCHOOLNAME=XAUT";
    if(putenv(my_env) == 0)
        printf("putenv successed!\n");
    else {
        perror("putenv failed");
        exit(EXIT_FAILURE);
    }
    
    char **ep;
    for(ep = environ; *ep != NULL; ++ep) {
        if(strcmp(*ep, my_env) == 0) {
            printf("fint it!\n");
            exit(EXIT_SUCCESS);
        }
    }
    printf("can't find it...\n");
    exit(EXIT_SUCCESS);
}