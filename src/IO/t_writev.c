#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAXSIZE 100

char buf[] = "#ABCDiov1\n";
char buf1[] = "$ABCDiov2\n";
char buf2[] = "&ABCDiov3\n";

int main(int argc, char *argv[]) {
    int fd;
    struct iovec iov[3];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }

    ssize_t numWrite, totRequired = 0;

    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND)) < 0) {
        fprintf(stderr, "open file error\n");
        exit(1);
    }

    iov[0].iov_base = buf;
    iov[0].iov_len = strlen(buf);
    totRequired += strlen(buf);

    iov[1].iov_base = buf1;
    iov[1].iov_len = strlen(buf1);
    totRequired += strlen(buf1);

    iov[2].iov_base = buf2;
    iov[2].iov_len = strlen(buf2);
    totRequired += strlen(buf2);

    if ((numWrite = writev(fd, iov, 3)) < 0) {
        fprintf(stderr, "writev error\n");
        exit(1);
    }

    if (numWrite < totRequired) {
        fprintf(stderr, "Write fewer bytes than requested\n");
        exit(1);
    }

    printf("total bytes requested: %ld; bytes write: %ld\n", (long)totRequired, (long)numWrite);
    exit(0);
}
