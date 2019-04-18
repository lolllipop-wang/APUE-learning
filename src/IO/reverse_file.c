#include <apue.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFFSIZE 256

char buf[MAXBUFFSIZE];

int main(int argc, char *argv[])
{
    if(argc != 3) 
        err_quit("usage: %s input_file output_file", argv[0]);
    
    off_t offset;
    int sub_fd[10];
    int input_fd, output_fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if((input_fd = open(argv[1], O_RDWR | O_EXCL)) < 0)
        err_sys("open input_file error");
    
    if((offset = lseek(input_fd, 0, SEEK_END)) < 0)
        err_sys("input seek error");
    if(lseek(input_fd, 0, SEEK_SET) < 0)
        err_sys("input seek error");

    
    int step = offset / 2 + 1;

    int byte_count = 0;
    int num_read = 0;
    int num_write = 0;
    int block_num = 2;
    int i;
    char file_name[10] = "0.data";
    for(i = 0; i < block_num; ++i) {
        file_name[0] = i + '0';
        if((sub_fd[i] = open(file_name, O_RDWR | O_CREAT, mode)) < 0)
            err_sys("create sub file %d error", i);
        
        num_read = 0;
        num_write = 0;
        while((num_read = read(input_fd, buf, MAXBUFFSIZE)) > 0) {
            if((num_write = write(sub_fd[i], buf, num_read)) != num_read)
                err_sys("write error");
            byte_count += num_read;
            if(byte_count >= (i + 1) * step)
                break;
        }

        close(sub_fd[i]);
    }

    if((output_fd = open(argv[2], O_RDWR | O_CREAT, mode)) < 0)
        err_sys("open output file error");
    
    for(i = block_num - 1; i >= 0; --i) {
        file_name[0] = i + '0';

        if((sub_fd[i] = open(file_name, O_RDWR | O_CREAT, mode)) < 0)
            err_sys("create sub file %d error", i);
        
        while((num_read = read(sub_fd[i], buf, MAXBUFFSIZE)) > 0) {
            if((num_write = write(output_fd, buf, num_read)) != num_read)
                err_sys("write error");
        }

        close(sub_fd[i]);
    }
    
    exit(0);
}