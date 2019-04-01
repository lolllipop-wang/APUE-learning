#include <apue.h>
#include <fcntl.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if(argc != 4) 
        err_quit("Usage: %s file offset text", argv[0]);
    
    int fd;
    off_t offset;
    ssize_t totRequired = strlen(argv[3]);
    ssize_t numWrite;

    if((fd = open(argv[1], O_RDWR | O_CREAT)) < 0)
        err_sys("open file error");
    
    offset = atol(argv[2]);

    if(offset < 0) 
        err_quit("offset must greater than 0");
    
    if((numWrite = pwrite(fd, argv[3], totRequired, offset)) < 0)
        err_sys("pwrite error");
    if(numWrite < totRequired)
        printf("Write fewer bytes than requested");

    printf("total bytes requested: %ld; bytes write: %ld\n", 
            (long)totRequired, (long)numWrite);
    exit(EXIT_SUCCESS);
}