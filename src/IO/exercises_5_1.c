/**
 * 《Linux/UNIX系统编程手册》第五章课后练习1
 */
#define _FILE_OFFSET_BITS 64
#include <apue.h>
#include <fcntl.h>

#define BUFFSIZE 1024

char buf[BUFFSIZE];

int
main(int argc, char *argv[])
{

    if(argc != 3)
        err_quit("Usage: %s large_file_name offset\n", argv[0]);
    
    int fd, i;
    off_t offset;
    ssize_t numRead = 0;
    mode_t mode = S_IRUSR | S_IWUSR;
    if((fd = open(argv[1], O_RDWR | O_CREAT, mode)) < 0)
        err_sys("open file error");
    
    offset = atoll(argv[2]);
    if(offset < 0)
        err_quit("offset must greater than zero\n");
    
    if(lseek(fd, offset, SEEK_SET) < 0)
        err_sys("lseek error");

    numRead = read(fd, buf, BUFFSIZE);

    if(numRead < 0)
        err_sys("read error");
    if(numRead < BUFFSIZE)
        printf("read fewer bytes\n");
    
    printf("Read: %lld bytes\n", (ssize_t)numRead);

    for(i = 0; i < numRead; ++i) {
        printf("%02X ", buf[i]);
        if(i != 0 && i % 16 == 0)
            printf("\n");
    }
    printf("\n");
    exit(EXIT_SUCCESS);
}