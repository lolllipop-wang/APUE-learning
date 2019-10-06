/**
 * 测试不同大小的缓冲区对IO速度的影响
 *
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if !defined(BUFFSIZE)
#define BUFFSIZE 4096
#endif

char buf[BUFFSIZE];

int main(int argc, char *argv[]) {
  int ifd, ofd;
  ssize_t nread, nwrite;
  int bytes = 0;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  if (argc != 3) {
    fprintf(stderr, "usage: %s input output\n", argv[0]);
    exit(1);
  }

  if ((ifd = open(argv[1], O_RDONLY)) < 0) {
    fprintf(stderr, "can't ope file %s\n", argv[1]);
    exit(1);
  }

  if ((ofd = open(argv[2], O_RDWR | O_CREAT, mode)) < 0) {
    fprintf(stderr, "can't open file %s\n", argv[2]);
    exit(1);
  }

  printf("BUFFSIZE: %d\n", BUFFSIZE);

  while ((nread = read(ifd, buf, BUFSIZ)) > 0) {
    if ((nwrite = write(ofd, buf, nread)) < 0) {
      fprintf(stderr, "write error\n");
      exit(1);
    }

    bytes += nwrite;
    if (bytes >= 1024 * 1024 * 100) /* read 100MB */
      break;
  }

  if (nread < 0) {
    fprintf(stderr, "read error\n");
    exit(1);
  }

  exit(0);
}
