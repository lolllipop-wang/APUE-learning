#include <apue.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int istack = 111;

    switch (vfork())
    {
        case -1:
            err_sys("vfork error");
            break;
        case 0:
            sleep(3);

            write(STDOUT_FILENO, "child executing!\n", 30);

            istack *= 3;
            _exit(0);
        default:

            write(STDOUT_FILENO, "parent executing!\n", 30);
            printf("istack = %d after fork\n", istack);
            exit(0);
    }
}