#include <apue.h>

int main(int argc, char *argv[]) {
  siginfo_t info;
  sigset_t blocking_mask;
  int blocking_time;
  int sig;
  blocking_time = argc >= 2 ? atoi(argv[1]) : 10;

  printf("%s: PID = %ld, UID = %ld\n", argv[0], (long)getpid(), (long)getuid());

  sigfillset(&blocking_mask);

  if (sigprocmask(SIG_SETMASK, &blocking_mask, NULL) < 0)
    err_sys("sigprocmask error");

  printf("%s: blocking all signals, sleep %d seconds\n", argv[0],
         blocking_time);
  sleep(blocking_time);
  printf("%s: weak up!!!\n", argv[0]);

  for (;;) {
    sig = sigwaitinfo(&blocking_mask, &info);
    if (sig < 0)
      err_sys("sigwaitinfo error");

    if (sig == SIGINT || sig == SIGTERM) {
      printf("caught signal %d(%s)\n", sig, strsignal(sig));
      exit(0);
    }

    char *sig_code_buf = info.si_code == SI_USER
                             ? "SI_USER"
                             : info.si_code == SI_QUEUE ? "SI_QUEUE" : "other";

    printf("caught signal %d\n", sig);
    printf("\tsi_signo = %d(%s), si_code = %d(%s), si_value = %d\n",
           info.si_signo, strsignal(info.si_signo), info.si_code, sig_code_buf,
           info.si_value.sival_int);
    printf("\tPID = %ld, UID = %ld\n", (long)info.si_pid, (long)info.si_uid);
  }
  exit(0);
}