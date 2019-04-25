#include <apue.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if(argc > 1) {
        int flags = fcntl(STDOUT_FILENO, F_GETFD);
        if(flags == -1)
            err_sys("fcntl get flags error");
        
        flags |= FD_CLOEXEC;

        if(fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)
            err_sys("fcntl seet flags error");
        
    }

    execlp("ls", "ls", "-l", argv[0], (char *)0);
    err_sys("execlp errpr");
    exit(0);
}