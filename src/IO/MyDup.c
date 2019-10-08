/// dup 和 dup2的实现
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

int my_dup(int oldfd);
int my_dup2(int oldfd, int newfd);

int main(int argc, char *argv[]) { exit(EXIT_SUCCESS); }

int my_dup(int oldfd) {
    /*
    if(fcntl(oldfd, F_GETFL) < 0)
        err_sys("old fd dosen't exisit");

    int newfd = fcntl(oldfd, F_DUPFD, 0);

    if(newfd < 0)
        err_sys("dup error");

    return newfd;
    */
    if (fcntl(oldfd, F_GETFL) < 0) {
        errno = EBADF;
        return -1;
    }
    int fd = fcntl(oldfd, F_DUPFD, 0);

    return fd;
}

int my_dup2(int oldfd, int newfd) {
    /*
    if(fcntl(oldfd, F_GETFL) < 0)
        err_sys("old fd dosen't exisit");

    if(oldfd == newfd)
        return oldfd;

    newfd = fcntl(oldfd, F_DUPFD, newfd);

    if(newfd < 0)
        err_sys("dup2 error");

    if(close(oldfd) < 0)
        err_sys("close old fd error");
    */
    int fd;
    if (oldfd == newfd) {
        if (fcntl(oldfd, F_GETFL) < 0) {
            errno = EBADF;
            fd = -1;
        } else
            fd = oldfd;
    } else {
        close(newfd);
        fd = fcntl(oldfd, F_DUPFD, newfd);
    }
    return fd;
}