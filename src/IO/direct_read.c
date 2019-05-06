/**
 * 
 * 绕过缓冲区高速缓存直接IO
 */
#define _GNU_SOURCE
#include <apue.h>
#include <fcntl.h>
#include <malloc.h>

int main(int argc, char *argv[])
{
    int fd;
    int nread;
    char *buf;
    off_t offset;
    int length, alignment;


    if (argc < 3)
        err_quit("usage: %s file nbytes [offset] [alignment]");

    length = atoi(argv[2]);
    offset = argc > 3 ? atol(argv[3]) : 0;
    alignment = argc > 4 ? atoi(argv[4]) : 4096;

    if ( (fd = open(argv[1], O_DIRECT | O_RDONLY)) < 0)
        err_sys("can't open file %s", argv[1]);
    
    buf = (char *)memalign(alignment * 2, length + alignment) + alignment;

    if (buf == NULL)
        err_sys("memalign error");
    
    if (lseek(fd, offset, SEEK_SET) < 0)
        err_sys("seek error");
    
    if ( (nread = read(fd, buf, length)) < 0)
        err_sys("read error");
    
    printf("read %d bytes\n", nread);
    exit(0);
}