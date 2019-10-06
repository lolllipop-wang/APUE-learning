#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char buf[16440];

int main() {
  int fd;
  if ((fd = creat("file.nohole", 0666)) < 0) {
    fprintf(stderr, "create file error\n");
    exit(1);
  }

  if (write(fd, buf, 16394) != 16394) {
    fprintf(stderr, "write error\n");
    exit(1);
  }

  return 0;
}
