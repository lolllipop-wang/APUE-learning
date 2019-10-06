#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s file offset text\n", argv[0]);
    exit(1);
  }

  int fd;
  off_t offset;
  ssize_t totRequired = strlen(argv[3]);
  ssize_t numWrite;

  if ((fd = open(argv[1], O_RDWR | O_CREAT)) < 0) {
    fprintf(stderr, "open file error\n");
    exit(1);
  }

  offset = atol(argv[2]);

  if (offset < 0) {
    fprintf(stderr, "offset must greater than 0\n");
    exit(1);
  }

  if ((numWrite = pwrite(fd, argv[3], totRequired, offset)) < 0) {
    fprintf(stderr, "pwrite error\n");
    exit(1);
  }

  if (numWrite < totRequired)
    printf("Write fewer bytes than requested");

  printf("total bytes requested: %ld; bytes write: %ld\n", (long)totRequired,
         (long)numWrite);
  exit(EXIT_SUCCESS);
}
