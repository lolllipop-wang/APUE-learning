/**
 * race condition
 */
#include <apue.h>

static void charatatime(char *);

int main()
{
    pid_t pid;


    switch ((pid = fork()))
    {
        case -1:                    /* failed to create child process */
            err_sys("fork error");

        case 0:                     /* child */
            charatatime("output from child\n");

        default:                    /* parent */
            charatatime("output from parent\n");
            break;
    }
    exit(0);
}

static void charatatime(char *buf)
{
    char *ptr;
    int c;
    setbuf(stdout, NULL);
    for(ptr = buf; (c = *ptr++) != 0; ) {
        putc(c, stdout);
    }
}
