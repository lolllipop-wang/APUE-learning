#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("can't seek\n");
    else
        printf("can seek\n");
    return 0;
}
