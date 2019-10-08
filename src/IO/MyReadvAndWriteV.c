#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/uio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXSIZE 100

ssize_t my_readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt);

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

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    /**
     * my_pwrite
     */
    printf("##### my_writev begin #####\n");
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND, mode)) < 0) {
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

    if (numWrite < totRequired)
        printf("Write fewer bytes than requested\n");

    printf("total bytes requested: %ld; bytes write: %ld\n", (long)totRequired, (long)numWrite);

    close(fd);
    printf("##### my_writev end #####\n");

    /**
     * my_readv
     */
    printf("##### my_readv begin #####\n");
    ssize_t numRead;
    int x;

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "open file error\n");
        exit(1);
    }

    totRequired = 0;
    iov[0].iov_base = &x;
    iov[0].iov_len = sizeof(x);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = buf;
    iov[1].iov_len = strlen(buf);
    totRequired += iov[1].iov_len;

    if ((numRead = readv(fd, iov, 2)) < 0) {
        fprintf(stderr, "readv error\n");
        exit(1);
    }

    if (numRead < totRequired)
        printf("read fewer bytes than requested\n");

    printf("total bytes requested: %ld; bytes read: %ld\n", (long)totRequired, (long)numRead);
    printf("##### my_readv end #####\n");
    exit(EXIT_SUCCESS);
}

ssize_t my_readv(int fd, const struct iovec *iov, int iovcnt) {
    if (iov == NULL || iovcnt == 0)
        return 0;

    if (fcntl(fd, F_GETFL) < 0) {
        errno = EBADF;
        return -1;
    }

    ssize_t num_read = 0;

    int i;
    for (i = 0; i < iovcnt; ++i) {
        num_read += read(fd, iov[i].iov_base, iov[i].iov_len);
    }

    if (num_read < 0)
        return -1;
    return num_read;
}

ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt) {
    if (iov == NULL || iovcnt == 0)
        return 0;

    if (fcntl(fd, F_GETFL) < 0) {
        errno = EBADF;
        return -1;
    }

    ssize_t num_write = 0;
    int i;
    for (i = 0; i < iovcnt; ++i)
        num_write += write(fd, iov[i].iov_base, iov[i].iov_len);

    if (num_write < 0)
        return -1;
    return num_write;
}
