#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXBUFFLENGTH 4096

char buf[MAXBUFFLENGTH];

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s file count offset\n", argv[1]);
    exit(1);
  }

  int fd;
  int count;
  char *it;
  ssize_t numRead;
  off_t offset;
  off_t real_offset;
  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    fprintf(stderr, "open file error\n");
    exit(1);
  }

  offset = atol(argv[3]);
  count = atoi(argv[2]);
  if (offset < 0) {
    fprintf(stderr, "offset must greater than 0\n");
    exit(1);
  }

  if (count < 0) {
    fprintf(stderr, "count must greater than 0\n");
    exit(1);
  }

  if (count > MAXBUFFLENGTH) {
    fprintf(stderr, "count too large\n");
    exit(1);
  }

  if ((real_offset = lseek(fd, 0, SEEK_END)) < 0) {
    fprintf(stderr, "lseek error\n");
    exit(1);
  }

  if (count > real_offset) {
    fprintf(stderr, "count must be shorter than file length\n");
    exit(1);
  }

  if ((numRead = pread(fd, buf, count, offset)) < 0) {
    fprintf(stderr, "pread error\n");
    exit(1);
  }

  printf("total bytes requested: %ld, bytes read: %ld\n", (long)count, numRead);
  if (numRead < count) {
    fprintf(stderr, "Read fewer bytes than requested\n");
    exit(1);
  }

  for (it = buf; it != (buf + numRead); it++)
    printf("%c", *it);
  printf("\n");
  exit(EXIT_SUCCESS);
}
