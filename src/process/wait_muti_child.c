#include <apue.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
        err_quit("usgae: %s sec1 sec2 sec3...");

    setbuf(stdout, NULL);

    int i, num_dead = 0;
    time_t t;
    pid_t pid;
    char *time_now = NULL;
    for(i = 1; i < argc; ++i) {
        switch (fork())
        {
            case -1:
                err_sys("fork error");
                break;
            case  0:
                time(&t);
                time_now = ctime(&t);
                time_now[strlen(time_now) - 1] = '\0';
                printf("[%s] child %d started with PID %ld sleeping %s seconds\n",
                    time_now, i, (long)getpid(), argv[i]);
                fflush(NULL);
                sleep(atoi(argv[i]));
                exit(0);
            default:
                break;
        }
    }

    for(;;) {
        pid = wait(NULL);
        if(pid == -1) {
            if(errno = ECHILD) {
                printf("No more child, Bye bye\n");
                exit(0);
            } 
            else 
                err_sys("wait error");
        }
        num_dead++;
        time(&t);
        time_now = ctime(&t);
        time_now[strlen(time_now) - 1] = '\0';
        printf("[%s] wait returned child PID = %ld (num dead = %d)\n",
                    time_now, (long)pid, num_dead);
    }
    exit(0);
}