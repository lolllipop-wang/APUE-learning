#include <apue.h>

static volatile sig_atomic_t done = 0;

static void handler_sigcont(int sig) {
  printf("caught signal %d(%s)\n", sig, strsignal(sig));
}

static void handler_sigint(int sig) {
  printf("caught signal %d(%s)\n", sig, strsignal(sig));
  done = 1;
}

int main(int argc, char *argv[]) {
  printf("%s: PID = %ld, UID = %ld\n", argv[0], (long)getpid(), (long)getuid());

  struct sigaction sa_sigcont, sa_sigint;
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGCONT);

  /* block SIGCONT */
  if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0)
    err_sys("sigprocmask error");
  printf("blocking SIGCONT\n");

  /* SIGCONT */
  sa_sigcont.sa_flags = 0;
  sigfillset(&sa_sigcont.sa_mask);
  sa_sigcont.sa_handler = handler_sigcont;

  /* SIGINT */
  sa_sigint.sa_flags = SA_RESETHAND;
  sigfillset(&sa_sigint.sa_mask);
  sa_sigint.sa_handler = handler_sigint;

  printf("setting up SIGCONT handler...\n");
  if (sigaction(SIGCONT, &sa_sigcont, NULL) < 0)
    err_sys("setting SIGCONT handler error");

  printf("setting up SIGINT handler...\n");
  if (sigaction(SIGINT, &sa_sigint, NULL) < 0)
    err_sys("setting SIGINT handler error");

  while (done == 0)
    continue;
  sigemptyset(&mask);
  if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0)
    err_sys("sigprocmask error");
  printf("unblocking SIGCONT\n");

  for (;;)
    pause();

  exit(0);
}