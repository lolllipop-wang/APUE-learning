// 使用unlink接触文件的链接，程序结束时文件会被自动删除

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        err(EXIT_FAILURE, "Usage: %s <filename>", argv[0]);

    if (open(argv[1], O_RDWR) == -1)
        err(EXIT_FAILURE, "Failed open file %s", argv[1]);
    
    if (unlink(argv[1]) == -1)
        err(EXIT_FAILURE, "Failed unlink %s", argv[1]);
    
    printf("Unlink %s success!\n", argv[1]);
    
    for (int i = 5; i >= 0; i--) {
        sleep(1);
        printf("%s will be delete after %d seconds!\r", argv[0], i);
        fflush(NULL);
    }
    printf("\n");
    return 0;
}