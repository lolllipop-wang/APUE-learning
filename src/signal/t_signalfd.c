#include <apue.h>
#include <sys/signalfd.h>

int main(int argc, char *argv[])
{   
    int                     sfd;
    int                     num_read;
    sigset_t                mask;
    struct signalfd_siginfo sfd_info;

    printf("%s: PID = %ld, UID = %ld\n", argv[0], (long)getpid(), (long)getuid());

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGUSR1);

    if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        err_sys("sigprocmask error");

    sfd = signalfd(-1, &mask, 0);
    
    if(sfd < 0)
        err_sys("signalfd error");

    for(;;) {
        num_read = read(sfd, &sfd_info, sizeof(struct signalfd_siginfo));
        if(num_read < 0)
            err_sys("read error");
        
        printf("%s: caught signal %d(%s)\n", argv[0], sfd_info.ssi_signo, strsignal(sfd_info.ssi_signo));
        printf("\tPID = %ld, UID = %ld\n", (long)sfd_info.ssi_pid, (long)sfd_info.ssi_uid);
    }
    exit(0);
}