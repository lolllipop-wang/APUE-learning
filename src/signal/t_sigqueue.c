#define _POSIX_C_SOURCE 199309
#include <apue.h>



int main(int argc, char *argv[])
{
    if(argc < 4)
        err_quit("usage: %s PID sig data [num_sigs]", argv[0]);
    
    int sig, num_sigs, sig_data;
    union sigval sv;
    pid_t pid;

    printf("%s: PID = %ld, UID = %ld\n", argv[0], (long)getpid(), (long)getuid());

    pid = atol(argv[1]);
    sig = atoi(argv[2]);
    sig_data = atoi(argv[3]);
    num_sigs = argc > 4 ? atoi(argv[4]) : 1;

    while(num_sigs--) {
        sv.sival_int = sig_data + num_sigs;
        if(sigqueue(pid, sig, sv) < 0)
            err_sys("sigqueue error");
    }
    exit(0);
}