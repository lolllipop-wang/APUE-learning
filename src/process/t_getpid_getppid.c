#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{
    pid_t pid, ppid;
    pid = getpid();
    ppid = getppid();
    printf("process ID: %ld\n", (long)pid);
    printf("parent process ID: %ld\n", (long)ppid);
    exit(EXIT_SUCCESS);
}