/**
 *  large file
 */
#define _FILE_OFFSET_BITS 64
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 1024

char buf[BUFFSIZE];

int main(int argc, char *argv[]) {

  if (argc != 3) {
    fprintf(stderr, "Usage: %s large_file_name offset\n", argv[0]);
    exit(1);
  }

  int fd, i;
  off_t offset;
  ssize_t numRead = 0;
  mode_t mode = S_IRUSR | S_IWUSR;
  if ((fd = open(argv[1], O_RDWR | O_CREAT, mode)) < 0) {
    fprintf(stderr, "open file error");
    exit(1);
  }

  offset = atoll(argv[2]);
  if (offset < 0) {
    fprintf(stderr, "offset must greater than zero\n");
    exit(1);
  }

  if (lseek(fd, offset, SEEK_SET) < 0) {
    fprintf(stderr, "lseek error\n");
    exit(1);
  }

  numRead = read(fd, buf, BUFFSIZE);

  if (numRead < 0) {
    fprintf(stderr, "read error\n");
    exit(1);
  }

  if (numRead < BUFFSIZE)
    printf("read fewer bytes\n");

  printf("Read: %ld bytes\n", (ssize_t)numRead);

  for (i = 0; i < numRead; ++i) {
    printf("%02X ", buf[i]);
    if (i != 0 && i % 16 == 0)
      printf("\n");
  }
  printf("\n");
  exit(EXIT_SUCCESS);
}
