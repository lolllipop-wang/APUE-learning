/**
 * 可以用于复制任何一个UNIX普通文件
 * 使用shell重定向输入和输出。
 * cc -o cp ./cp.c -lapue
 * ./cp < inputfile > outputfile
 */

#include <apue.h>

#define BUFFSIZE 4096

int main() {
  int n;
  char buf[BUFFSIZE];

  while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
    if (write(STDOUT_FILENO, buf, n) != n) {
      err_sys("write error");
    }
  }

  if (n < 0) {
    err_sys("read error");
  }
  return 0;
}