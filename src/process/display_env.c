#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern char **environ;

int
main(int argc, char *argv[])
{
    char **ep;
    for(ep = environ; *ep != NULL; ++ep)
        printf("%s\n", *ep);
    exit(EXIT_SUCCESS);
}