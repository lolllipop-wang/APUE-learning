/**
 * 
 * 面试的时候被面试官问到的题
 * 将一个大文件翻转，直接使用lseek，甚至栈都不需要，只需要一个缓冲区。
 * 
 */

#include <apue.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFFSIZE 4096
#define SUBFILENUMS 4

char buf[MAXBUFFSIZE];

int  read_line(int fd, char *buf, int max_length);

int main(int argc, char *argv[])
{
    if(argc != 3) 
        err_quit("usage: %s input_file output_file", argv[0]);
    
    int input_fd, output_fd;    /* 输入和输出文件的文件描述符 */
    int num_read, num_write;    /* 每次正确读写的字节数      */
    int input_file_length;      /* 输入文件的长度           */

    /* 指定新建文件的权限 */
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;  

    if((input_fd = open(argv[1], O_RDONLY)) < 0)                               /* 打开输入文件 */
        err_sys("open input file error");

    /**
     * 如果输出文件不存在，则创建该文件
     */
    if((output_fd = open(argv[2], O_RDWR | O_CREAT, mode)) < 0)                 /* 打开输出文件 */
        err_sys("open output file error");

    if((input_file_length = lseek(input_fd, 0, SEEK_END)) < 0)      
        err_sys("seek error");
    
    printf("input file length: %d bytes\n", input_file_length);

    if(lseek(input_fd, 0, SEEK_SET) < 0)
        err_sys("seek error");

    if(lseek(output_fd, input_file_length, SEEK_SET) < 0)
        err_sys("seek error");


    while((num_read = read_line(input_fd, buf, MAXBUFFSIZE)) > 0) {

        if(lseek(output_fd, -num_read, SEEK_CUR) < 0)
            err_sys("seek error");

        if((num_write = write(output_fd, buf, num_read)) != num_read)
            err_sys("write error");
        
        if(lseek(output_fd, -num_read, SEEK_CUR) < 0)
            err_sys("seek error");
    }
    exit(0);
}

int read_line(int fd, char *buf, int max_length)
{
    if(buf == NULL)
        return -1;
    int cnt = 0;

    while(read(fd, buf, 1) > 0) {
        cnt++;
        buf++;
        if(*(buf - 1) == '\n' || cnt >= max_length)
            break;
    }
    return cnt;
}