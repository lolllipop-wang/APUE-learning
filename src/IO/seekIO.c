#include <apue.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);
        exit(1);
    }

    int fd, n, ap, len, i;
    char *buf;
    off_t offset;
    mode_t mod = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if ((fd = open(argv[1], O_RDWR | O_CREAT, mod)) == -1) {
        fprintf(stderr, "open file error\n");
        exit(1);
    }
    for (ap = 2; ap < argc; ++ap) {
        switch (argv[ap][0]) {
            case 'r':
            case 'R':
                len = atol(argv[ap] + 1);
                buf = (char *)malloc(len);
                if (buf == NULL) {
                    fprintf(stderr, "malloc error\n");
                    exit(1);
                }
                if ((n = read(fd, buf, len)) < 0) {
                    fprintf(stderr, "read error\n");
                    exit(1);
                }
                if (n == 0) {
                    fprintf(stderr, "end of file\n");
                    exit(1);
                }

                printf("%s: \n", argv[ap]);

                for (i = 0; i < n; ++i) {
                    if (argv[ap][0] == 'r')
                        printf("%c", buf[i]);
                    else
                        printf("%02x ", buf[i]);
                }
                printf("\n");
                free(buf);
                break;
            case 'w':
                if ((n = write(fd, argv[ap] + 1, strlen(argv[ap] + 1))) < 0) {
                    fprintf(stderr, "write error\n");
                    exit(1);
                }

                printf("%s : worte %ld bytes\n", argv[ap], (long)n);
                break;
            case 's':
                offset = atol(argv[ap] + 1);
                if ((lseek(fd, offset, SEEK_SET)) < 0) {
                    fprintf(stderr, "lseek error\n");
                    exit(1);
                }

                printf("%s: seek successed!\n", argv[ap]);
                break;
            default:
                fprintf(stderr, "We have a big truble\n");
                exit(1);
        }
    }
    exit(0);
}
