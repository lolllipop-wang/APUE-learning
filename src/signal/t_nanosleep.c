/**
 * 定时器与休眠
 * 高分辨率休眠nanosleep
 */
#include <apue.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>

static void handler(int sig){}      /* 仅用来打断nanosleep */

int main(int argc, char *argv[])
{
    int state;
    struct sigaction sa;
    struct timeval start, finish;
    struct timespec request, remain;

    if(argc != 3)
        err_quit("usage: %s sec usec", argv[0]);
    
    request.tv_sec = atoi(argv[1]);
    request.tv_nsec = atoi(argv[2]);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if(sigaction(SIGINT, &sa, NULL) == -1)
        err_sys("setting SIGINT handler error");
    
    if(gettimeofday(&start, NULL) == -1)
        err_sys("gettimeofday error");
    printf("AhAhAh...., I'am going to sleep\n");

    for(;;) {
        state = nanosleep(&request, &remain);

        if(state == -1 && errno != EINTR)
            err_sys("nanosleep error");
        
        if(gettimeofday(&finish, NULL) == -1)
            err_sys("gettimeofday error");

        printf("Sleep %9.6f seconds\n", (finish.tv_sec - start.tv_sec) + 
                (finish.tv_usec - start.tv_usec) / 1000000.0);
        
        if(state == 0)
            break;
        
        printf("Remaining %2ld.%09ld\n", (long)remain.tv_sec, (long)remain.tv_nsec);
        
        request = remain;
    }
    printf("Sleep complete\n");
    exit(0);
}