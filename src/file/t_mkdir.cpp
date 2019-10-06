// 创建目录
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc != 2)
    err(EXIT_FAILURE, "Usage: %s dirname", argv[0]);

  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  if (mkdir(argv[1], mode) == -1)
    err(EXIT_FAILURE, "Failed create %s", argv[1]);

  printf("Create directory %s success!\n", argv[1]);
  return 0;
}