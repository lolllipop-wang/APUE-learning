/**
 *  错误的使用系统调用和stdio库混合IO
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char buf1[] = "This is message 1";
  char buf2[] = "This is message 2";

  printf("%s", buf1);
  write(STDOUT_FILENO, buf2, sizeof(buf2));
  exit(0);
}