#define _LARGEFILE64_SOURCE
#include <apue.h>
#include <fcntl.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if(argc != 3)
        err_quit("Usage: %s pathname offset", argv[0]);
    off64_t offset;
    int fd;

    if((fd = open64(argv[1], O_RDWR, S_IRUSR | S_IWUSR)) < 0)
        err_sys("open large file error");
    offset = atoll(argv[2]);

    if(lseek64(fd, offset, SEEK_SET) < 0)
        err_sys("seek error");
    if(write(fd, "test", 4) < 0)
        err_sys("write error");
    exit(EXIT_SUCCESS);
}