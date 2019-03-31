#include <apue.h>
#include <fcntl.h>
#include <string.h>

#define MAXBUFFLENGTH 4096

char buf[MAXBUFFLENGTH];

int 
main(int argc, char *argv[])
{
    if(argc != 4)
        err_quit("Usage: %s file count offset", argv[1]);
    
    int fd;
    int count;
    ssize_t numRead;
    off_t offset;
    off_t real_offset;
    if((fd = open(argv[1], O_RDONLY)) < 0)
        err_sys("open file error");
    
    offset = atol(argv[3]);
    count  = atoi(argv[2]);
    if(offset < 0) 
        err_quit("offset must greater than 0");

    if(count < 0)
        err_quit("count must greater than 0");

    if(count > MAXBUFFLENGTH)
        err_quit("count too large");

    if((real_offset = lseek(fd, 0, SEEK_END)) < 0)
        err_sys("lseek error");

    if(count > real_offset)
        err_quit("count must be shorter than file length");

    if((numRead = pread(fd, buf, count, offset)) < 0)
        err_sys("pread error");
    printf("total bytes requested: %ld, bytes read: %ld\n", (long)count, numRead);
    if(numRead < count)
        err_sys("Read fewer bytes than requested");
    for(char *i = buf; i != (buf + numRead); i++)
        printf("%c", *i);
    printf("\n");
    exit(EXIT_SUCCESS);
}