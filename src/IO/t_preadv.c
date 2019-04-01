#include <apue.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

int
main(int argc, char *argv[])
{
    if(argc != 3)
        err_quit("Usage: %s file offset", argv[0]);
    int fd;
    off_t offset;
    struct iovec iov[2];

    int data1;
    char buf[10];

    ssize_t numRead;
    ssize_t totRequired = 0; 

    offset = atol(argv[2]);

    if(offset < 0)
        err_quit("offset must greater than 0");

    if((fd = open(argv[1], O_RDONLY)) < 0)
        err_sys("open file error");
    
    iov[0].iov_base = &data1;
    iov[0].iov_len  = sizeof(int);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = buf;
    iov[1].iov_len  = sizeof(buf);
    totRequired += iov[1].iov_len;

    if((numRead = preadv(fd, iov, 2, offset)) < 0)
        err_sys("preadv error");

    if(numRead < totRequired)
        err_sys("read fewer bytes than requested");
    
    printf("total bytes requested: %ld; bytes read: %ld\n",
            (long) totRequired, (long)numRead);



    exit(EXIT_SUCCESS);
}