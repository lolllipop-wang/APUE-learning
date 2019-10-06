/**
 * 在源代码开头添加注释
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd;
  ssize_t num_write;
  mode_t mode = S_IRUSR | S_IWUSR;
  char buf[] = "\n/*这是我用程序添加的一行注释。。。*/";
  char file[] = "./main.cpp";

  if ((fd = open(file, O_RDWR | O_CREAT | O_APPEND, mode)) < 0) {
    fprintf(stderr, "open file error\n");
    exit(1);
  }

  if (lseek(fd, 0, SEEK_SET) < 0) {
    fprintf(stderr, "seek error\n");
    exit(1);
  }

  num_write = write(fd, buf, strlen(buf));

  if (num_write < 0) {
    fprintf(stderr, "write error\n");
    exit(1);
  }

  if (num_write < strlen(buf))
    printf("write fewer bytes\n");

  printf("Write %lld bytes\n", (long long)num_write);
  exit(EXIT_SUCCESS);
}
