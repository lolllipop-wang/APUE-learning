// 创建符号链接

#include <err.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
        err(EXIT_FAILURE, "Usage: %s filename symname", argv[0]);
    
    if (symlink(argv[1], argv[2]) == -1)
        err(EXIT_FAILURE, "Failed create symlink to %s", argv[1]);
    printf("Create symlink success!\n");
    return 0;
}