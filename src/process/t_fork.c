#include <apue.h>

static int idata = 111;

int
main(int argc, char *argv[])
{
    int istack = 222;
    pid_t pid;

    switch (pid = fork())
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            idata *= 3;
            istack *= 3;
            break;
        default:
            sleep(3);
            break;
    }

    printf("PID: %ld (%s) idata = %d, istack = %d\n", (long)getpid(), 
            (pid == 0 ? "child" : "parent"), idata, istack);
    exit(0);
}