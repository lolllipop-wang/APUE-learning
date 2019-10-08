#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 4096

char buf[BUFFSIZE];

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    int InputFileNO, OutputFileNO;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if ((InputFileNO = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "open input file error\n");
        exit(1);
    }

    if ((OutputFileNO = open(argv[2], O_RDWR | O_CREAT | O_SYNC, mode)) < 0) {
        fprintf(stderr, "open output file error\n");
        exit(1);
    }

    size_t n;
    while ((n = read(InputFileNO, buf, BUFFSIZE)) > 0)
        if (write(OutputFileNO, buf, n) != n) {
            fprintf(stderr, "write error\n");
            exit(1);
        }

    if (n < 0) {
        fprintf(stderr, "read error\n");
        exit(1);
    }

    return 0;
}
