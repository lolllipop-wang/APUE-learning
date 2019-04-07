#include <apue.h>
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char *argv[])
{
    if(kill(0, SIGINT) != 0)
        err_sys("kill error");
    exit(EXIT_SUCCESS);
}