#include <apue.h>
#include <fcntl.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{   
    if(argc != 2)
        err_quit("usage: %s filename", argv[0]);
    
    int fd, flag, status;
    off_t offset;
    pid_t pid;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    if((fd = open(argv[1], O_RDWR | O_CREAT, mode)) < 0)
        err_sys("open file error");
    
    if((offset = lseek(fd, 0, SEEK_CUR)) < 0)
        err_sys("seek error");
    if((flag = fcntl(fd, F_GETFL)) < 0)
        err_sys("fcntl F_GETFL error");

    printf("offset before fork: %ld\n", (long)offset);

    printf("O_APPEND flag before fork: %s\n", 
            (flag & O_APPEND ? "on" : "off"));

    switch (pid = fork())
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            if(lseek(fd, 1000, SEEK_CUR) < 0)
                err_sys("lseek error");
            if((flag = fcntl(fd, F_GETFL)) < 0)
                err_sys("fcntl F_GETFL error");
            
            flag |= O_APPEND;

            if(fcntl(fd, F_SETFL, flag) < 0)
                err_sys("fcntl G_SETFL error");
            
            _exit(EXIT_SUCCESS);
        default:
            if((pid = waitpid(pid, &status, 0)) < 0)
                err_sys("waitpid error");
            
            printf("child has exit %s\n", 
                    (status == EXIT_SUCCESS ? "successed" : "failed"));
            
            if((offset = lseek(fd, 0, SEEK_CUR)) < 0)
                err_sys("seek error");
            
            if((flag = fcntl(fd, F_GETFL)) < 0)
                err_sys("fcntl F_GETFL error");
            
            printf("offset after fork int parent: %ld\n", (long)offset);

            printf("O_APPEND flag after fork in parent: %s\n", 
                    (flag & O_APPEND ? "on" : "off"));
            break;
    }

    exit(0);
}