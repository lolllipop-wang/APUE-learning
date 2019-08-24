/**
 * ===================================================================
 * #include <sys/stat.h>
 * 
 * int chmod(const char *pathname, mode_t mode);
 * 
 * int fchmod(int fd, mode_t mode);
 * 
 * int fchmodat(int fd, const char *pathname, mode_t mode, int flag);
 * 
 * Return 0 on success, or -1 on error.
 * ====================================================================
 */
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat st;


    if (stat("foo", &st) < 0)
        err(EXIT_FAILURE, "stat error for foo");

    if (chmod("foo", (st.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        err(EXIT_FAILURE, "chmod error for foo");
    
    /* rw-r--r-- */
    if(chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
        err(EXIT_FAILURE, "chmod error for bar");

    exit(0);
}