#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAXSIZE 100

int main(int argc, char *argv[]) {
  int fd;
  struct iovec iov[2];
  char buf[MAXSIZE];
  int x;

  ssize_t numRead, totRequired;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(1);
  }

  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    fprintf(stderr, "open file error\n");
    exit(1);
  }

  totRequired = 0;

  iov[0].iov_base = &x;
  iov[0].iov_len = sizeof(x);
  totRequired += iov[0].iov_len;

  iov[1].iov_base = buf;
  iov[1].iov_len = MAXSIZE;
  totRequired += iov[1].iov_len;

  if ((numRead = readv(fd, iov, 3)) < 0) {
    fprintf(stderr, "readv error\n");
    exit(1);
  }

  if (numRead < totRequired) {
    fprintf(stderr, "read fewer bytes than requested\n");
    exit(1);
  }

  printf("total bytes requested: %ld; bytes read: %ld\n", (long)totRequired,
         (long)numRead);
  exit(0);
}
