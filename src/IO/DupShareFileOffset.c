/// dup复制文件描述符共享文件偏移量
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXBUFFSIZE 4096

char buf[MAXBUFFSIZE];

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(1);
  }

  int fd, newfd;
  ssize_t step = 100;
  ssize_t num_read;
  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    fprintf(stderr, "open file error\n");
    exit(1);
  }

  printf("###########\n");
  printf("fd read begin\n");

  num_read = read(fd, buf, step);

  if (num_read < 0) {
    fprintf(stderr, "fd read error\n");
    exit(1);
  }

  if (num_read < step) {
    printf("fd read fewer bytes\n");
  }

  printf("fd read %lld bytes\n", (long long)num_read);

  buf[num_read] = '\0';

  printf("fd read:\n%s\nfd read end\n\n", buf);
  printf("###########\n");

  if ((newfd = dup(fd)) < 0) {
    fprintf(stderr, "dup error\n");
    exit(1);
  }

  printf("###########\n");
  printf("new fd read begin\n");

  num_read = read(newfd, buf, step);

  if (num_read < 0) {
    fprintf(stderr, "new fd read error\n");
    exit(1);
  }

  if (num_read < step)
    printf("new fd read fewer bytes\n");
  printf("new fd read %lld bytes\n", (long long)num_read);
  buf[num_read] = '\0';
  printf("new fd read:\n%s\nnew fd read end\n\n", buf);
  printf("###########\n");
  exit(EXIT_SUCCESS);
}