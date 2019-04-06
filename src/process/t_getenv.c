#include <apue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    if(argc <= 1)
        err_quit("usage: %s env_name\n", argv[0]);
    
    int i;
    for(i = 1; i < argc; ++i) {
        if(argv[i] != NULL) {
            printf("%s: %s\n", argv[i], getenv(argv[i]));
        }
    }
    exit(EXIT_SUCCESS);
}