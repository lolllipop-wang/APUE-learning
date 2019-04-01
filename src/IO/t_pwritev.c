#include <apue.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

int
main(int argc, char *argv[])
{
    if(argc < 4)
        err_quit("Usage: %s file offset buf1 buf2 buf3...", argv[0]);
    int fd, i;
    off_t offset;
    ssize_t numWrite, totRequired = 0;
    mode_t mod = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    struct iovec *iov = (struct iovec *)malloc(sizeof(struct iovec) * (argc - 3));

    if(iov == NULL)
        err_quit("malloc error");
    if((fd = open(argv[1], O_RDWR | O_CREAT, mod)) < 0) {
        free(iov);
        err_sys("open file error");
    }
        

    offset =  atol(argv[2]);

    if(offset < 0) {
        free(iov);
        err_quit("offset musr greadter than 0");
    }

    for(i = 0; i < argc - 3; ++i) {
        iov[i].iov_base = argv[i + 3];
        iov[i].iov_len = strlen(argv[i + 3]);
        totRequired += iov[i].iov_len;
    }

    if((numWrite = pwritev(fd, iov, (argc - 3), offset)) < 0) {
        free(iov);
        err_sys("pwritev error");
    }

    if(numWrite < totRequired) {
        free(iov);
        printf("requested: %ld, writed: %ld\n", totRequired, numWrite);
        err_quit("Write fewer bytes than requested");
    }
    
    printf("total bytes requested: %ld; bytes write: %ld\n", 
            (long)totRequired, (long)numWrite);

    free(iov);
    exit(EXIT_SUCCESS);
}