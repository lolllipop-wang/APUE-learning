/**
 * 《Linux/UNIX系统编程手册》第五章课后练习3
 */
#include <apue.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{

    if(argc < 3)
        err_quit("usage: %s filename buffer [x]", argv[0]);
    int fd;
    int no_x_open_oflags = O_RDWR | O_CREAT | O_APPEND;
    int x_open_oflags = O_RDWR | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR;
    ssize_t num_write;
    int buf_length = strlen(argv[2]);

    switch (argc)
    {
        case 3:
            printf("write data in normal mode!\n");
            if((fd = open(argv[1], no_x_open_oflags, mode)) < 0)
                err_sys("open file error");
            break;
        case 4:
            printf("write data in X mode!\n");
            if(argv[3][0] == 'X' && (fd = open(argv[1], x_open_oflags, mode)) < 0)
                err_sys("open file error");
            if(lseek(fd, 0, SEEK_END) < 0)
                err_sys("seek error");
            break;
        default:
            err_quit("usage: %s filename buffer [x]", argv[0]);
            break;
    }

    num_write = write(fd, argv[2], buf_length);
    if(num_write < 0)
        err_sys("write error");
    if(num_write < buf_length)
        printf("write fewer bytes");
    printf("write %lld bytes\n", (long long)num_write);
    exit(EXIT_SUCCESS);
}