#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main() {
    int fd;
    if ((fd = creat("file.hole", 0666)) < 0) {
        fprintf(stderr, "create file error\n");
        exit(1);
    }

    if (write(fd, buf1, 10) != 10) {
        fprintf(stderr, "write error\n");
        exit(1);
    }

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        fprintf(stderr, "lseek error\n");
        exit(1);
    }

    if (write(fd, buf2, 10) != 10) {
        fprintf(stderr, "write error\n");
        exit(1);
    }

    return 0;
}
