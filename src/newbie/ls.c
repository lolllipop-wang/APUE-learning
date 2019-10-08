/**
 * 第一个入门程序，可以列出一个目录下的文件及文件夹名称，
 * 是ls命令的一个简单实现。
 *
 * 运行时只能传入一个参数。
 * cc -o ls ./ls.c -lapue
 * ./ls /usr/include
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    DIR *dp;
    struct dirent *dirp;

    if (argc != 2) {
        fprintf(stderr, "usage: ls directory_name\n");
        exit(1);
    }

    if ((dp = opendir(argv[1])) == NULL) {
        fprintf(stderr, "can't open directory %s\n", argv[1]);
        exit(1);
    }

    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp->d_name);
    }

    closedir(dp);
    return 0;
}
