#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    if(clearenv() == 0)
        printf("clearenv successed!\n");
    else {
        perror("clearenv failed!\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}