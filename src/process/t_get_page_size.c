#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    /**
     * 获取内存页面的大小
     */
    long page_size = sysconf(_SC_PAGE_SIZE);
    printf("page size is %ld bytes\n", page_size);
    exit(EXIT_SUCCESS);
}