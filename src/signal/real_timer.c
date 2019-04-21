/**
 * 定时器与休眠
 */
#include <apue.h>
#include <time.h>
#include <sys/time.h>

static volatile sig_atomic_t got_alarm = 0;

static void display_times(const char *msg, int include_timer)
{
    struct itimerval itv;
    static struct timeval start;
    struct timeval curr;
    static int call_time = 0;

    if(call_time == 0 && gettimeofday(&start, NULL) == -1)
        err_sys("gettimeofday error");

    if(call_time % 20 == 0)
        printf("       Elapsed  Value Interval\n");

    if(gettimeofday(&curr, NULL) == -1)
        err_sys("gettimeof day error");
     
     printf("%-7s %6.2f", msg, 
            curr.tv_sec - start.tv_sec + (curr.tv_usec - start.tv_usec) / 1000000.0);

     if(include_timer) {
         if(getitimer(ITIMER_REAL, &itv) == -1)
            err_sys("getitimer error");
        
        printf("  %6.2f  %6.2f", itv.it_value.tv_sec + itv.it_value.tv_usec / 1000000.0,
                                 itv.it_interval.tv_sec + itv.it_interval.tv_usec / 1000000.0);
     }

     printf("\n");
     call_time++;
}

static void handler(int sig)
{
    got_alarm = 1;
}

int main(int argc, char *argv[])
{
    struct itimerval itv;
    clock_t prev_clock;
    int max_sigs;
    int sig_cnt;
    struct sigaction sa;

    if(argc != 5)
        err_sys("usage: %s secs usecs int-secs int-usecs", argv[0]);

    sig_cnt = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    sa.sa_flags = 0;

    if(sigaction(SIGALRM, &sa, NULL) < 0) 
        err_sys("setting SIGALRM handler error");
    
    max_sigs = (itv.it_interval.tv_sec == 0 && 
                itv.it_interval.tv_usec == 0) ? 1 : 3;
    
    itv.it_value.tv_sec = atoi(argv[1]);
    itv.it_value.tv_usec = atoi(argv[2]);
    itv.it_interval.tv_sec = atoi(argv[3]);
    itv.it_interval.tv_usec = atoi(argv[4]);

    display_times("START:", 0);

    if(setitimer(ITIMER_REAL, &itv, NULL) == -1)
        err_sys("setitmer error");
    
    prev_clock = clock();
    sig_cnt = 0;

    for(;;) {

        while(((clock() - prev_clock) * 10 / CLOCKS_PER_SEC) < 5) {
            if(got_alarm) {
                got_alarm = 0;
                display_times("ALARM:", 1);

                sig_cnt++;
                if(sig_cnt >= max_sigs) {
                    printf("Than's all fokls\n");
                    exit(0);
                }
            }
        }

        prev_clock = clock();
        display_times("Main: ", 1);
    }
    exit(0);
}