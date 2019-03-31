#include <apue.h>
#include <fcntl.h>
#include <sys/uio.h>

#define MAXSIZE 100

int
main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[2];
    char buf[MAXSIZE];
    int x;

    ssize_t numRead, totRequired;

    if(argc != 2)
        err_sys("Usage: %s file", argv[0]);
    
    if((fd = open(argv[1], O_RDONLY)) < 0)
        err_sys("open file error");
    
    totRequired = 0;

    iov[0].iov_base = &x;
    iov[0].iov_len = sizeof(x);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = buf;
    iov[1].iov_len = MAXSIZE;
    totRequired += iov[1].iov_len;

    if((numRead = readv(fd, iov, 3)) < 0)
        err_sys("readv error");

    if(numRead < totRequired)
        err_sys("read fewer bytes than requested");
    
    printf("total bytes requested: %ld; bytes read: %ld\n",
            (long) totRequired, (long)numRead);
    exit(0);
}