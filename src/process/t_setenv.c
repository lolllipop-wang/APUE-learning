#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

int
main(int argc, char *argv[])
{
    char *my_env = "SCHOOL=XAUT";
    if(setenv("SCHOOL", "XAUT", 0) == 0)
        printf("setenv successed!\n");
    else {
        perror("setenv failed");
        exit(EXIT_FAILURE);
    }

    char **ep;
    int flag = 0;
    for(ep = environ; *ep != NULL; ++ep) {
        if(strcmp(*ep, my_env) == 0) {
            printf("fint it!\n");
            flag = 1;
        }
    }

    if(flag == 0) {
        printf("can't find it\n");
        exit(EXIT_FAILURE);
    }

    if(unsetenv("SCHOOL") == 0)
        printf("unsetenv successed!\n");
    else {
        perror("unsetenv failed");
        exit(EXIT_FAILURE);
    }
        
    exit(EXIT_SUCCESS);
}