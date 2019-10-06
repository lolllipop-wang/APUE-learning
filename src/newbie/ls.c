/**
 * 第一个入门程序，可以列出一个目录下的文件及文件夹名称，
 * 是ls命令的一个简单实现。
 *
 * 运行时只能传入一个参数。
 * cc -o ls ./ls.c -lapue
 * ./ls /usr/include
 */

#include <apue.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
  DIR *dp;
  struct dirent *dirp;

  if (argc != 2) {
    err_quit("usage: ls directory_name");
  }

  if ((dp = opendir(argv[1])) == NULL) {
    err_sys("can't open directory %s", argv[1]);
  }

  while ((dirp = readdir(dp)) != NULL) {
    printf("%s\n", dirp->d_name);
  }

  closedir(dp);
  return 0;
}