/**
 * 测试不同大小的缓冲区对IO速度的影响
 * 
 * cc -o IO_speed -lapue -D BUFFSIZE=4096
 * time ./IO_speed
 */
#include <apue.h>
#include <fcntl.h>

#if !defined(BUFFSIZE)
#define BUFFSIZE 4096
#endif

char buf[BUFFSIZE];

int main(int argc, char *argv[])
{
    int ifd, ofd;
    int nread;
    int nwrite;
    int bytes = 0;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                  S_IROTH | S_IWOTH;
    
    if (argc != 3)
        err_quit("usage: %s input output",argv[1], argv[2]);
    
    if ( (ifd = open(argv[1], O_RDONLY)) < 0)
        err_sys("can't ope file %s", argv[1]);

    if ( (ofd = open(argv[2], O_RDWR | O_CREAT, mode)) < 0)
        err_sys("can't open file %s", argv[2]);
    
    printf("BUFFSIZE: %d\n", BUFFSIZE);

    while ( (nread = read(ifd, buf, BUFSIZ)) > 0) {
        if ( (nwrite = write(ofd, buf, nread)) < 0)
            err_sys("write error");
        bytes += nwrite;
        if(bytes >= 1024 * 1024 * 100)  /* read 100MB */
            break;
    }
        
    
    if(nread < 0)
        err_sys("read error");
    exit(0);
}