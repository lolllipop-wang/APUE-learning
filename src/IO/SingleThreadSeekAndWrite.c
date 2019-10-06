/**
 * 《Linux/UNIX系统编程手册》第五章课后练习3
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "usage: %s filename buffer [x]\n", argv[0]);
    exit(1);
  }

  int fd;
  int no_x_open_oflags = O_RDWR | O_CREAT | O_APPEND;
  int x_open_oflags = O_RDWR | O_CREAT;
  mode_t mode = S_IRUSR | S_IWUSR;
  ssize_t num_write;
  size_t buf_length = strlen(argv[2]);

  switch (argc) {
  case 3:
    printf("write data in normal mode!\n");
    if ((fd = open(argv[1], no_x_open_oflags, mode)) < 0) {
      fprintf(stderr, "open file error\n");
      exit(1);
    }

    break;
  case 4:
    printf("write data in X mode!\n");
    if (argv[3][0] == 'X' || (fd = open(argv[1], x_open_oflags, mode)) < 0) {
      fprintf(stderr, "open file error\n");
      exit(1);
    }

    if (lseek(fd, 0, SEEK_END) < 0) {
      fprintf(stderr, "seek error\n");
      exit(1);
    }
    break;
  default:
    fprintf(stderr, "usage: %s filename buffer [x]\n", argv[0]);
    exit(1);
  }

  num_write = write(fd, argv[2], buf_length);
  if (num_write < 0) {
    fprintf(stderr, "write error\n");
    exit(1);
  }

  if (num_write < buf_length)
    printf("write fewer bytes\n");
  printf("write %lld bytes\n", (long long)num_write);
  exit(EXIT_SUCCESS);
}
