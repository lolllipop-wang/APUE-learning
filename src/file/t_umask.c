/**
 * ====================================
 * #include <sys/stat.h>
 *
 * mode_t umask(mode_t cmask);
 * ====================================
 *
 */
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    mode_t RWRWRW = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    umask(0);
    if (creat("foo", RWRWRW) < 0)
        err(EXIT_FAILURE, "create error for foo");

    umask(RWRWRW);
    if (creat("bar", RWRWRW) < 0)
        err(EXIT_FAILURE, "create error for bar");
    exit(0);
}