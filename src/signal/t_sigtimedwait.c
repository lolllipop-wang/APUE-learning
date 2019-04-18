/**
 * 
 * 有问题，为解决。
 * 设置了时间但是sigtimedwait立即返回。
 */
#include <apue.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    siginfo_t       info;
    sigset_t        blocking_mask;
    time_t          blocking_time;
    int             sig;
    struct timespec timeout;
    timeout.tv_sec = 5;      
    blocking_time = argc >= 2 ? atoi(argv[1]) : 10;

    printf("%s: PID = %ld, UID = %ld\n", argv[0], (long)getpid(), (long)getuid());

    sigemptyset(&blocking_mask);
    sigaddset(&blocking_mask, SIGINT);

    if(sigprocmask(SIG_BLOCK, &blocking_mask, NULL) < 0)
        err_sys("sigprocmask error");
    
    printf("%s: blocking all signals, sleep %d seconds\n", argv[0], blocking_time);
    sleep(blocking_time);
    printf("%s: weak up!!!!\n", argv[0]);
    timeout.tv_sec = 5; 
    timeout.tv_sec = 0;
    for(;;) {
        sig = sigtimedwait(&blocking_mask, &info, &timeout);
        if(sig < 0) {
            if(errno = EAGAIN)
                printf("%s: time out\n", argv[0]);
            else 
                err_sys("sigtimedwait error");
            break;
        }

        if(sig == SIGINT || sig == SIGTERM) {
            printf("%s: caught signal %d(%s)\n", argv[0], sig, strsignal(sig));
            exit(0);
        }

        char *sig_code_buf = info.si_code == SI_QUEUE ? "SI_QUEUE" :
                             info.si_code == SI_USER  ? "SI_USER"  : "other";
        
        printf("%s: caught signal %d(%s)\n", argv[0], sig, strsignal(sig));
        printf("\tsi_signo = %d(%s), si_code = %d(%s), si_value = %d\n",
                info.si_signo, strsignal(info.si_signo), info.si_code, 
                sig_code_buf, info.si_value.sival_int);
        printf("\tPID = %ld, UID = %ld\n", (long)info.si_pid, (long)info.si_uid);
    }

    exit(0);
}