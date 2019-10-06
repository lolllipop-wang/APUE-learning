#include <apue.h>
#include <signal.h>

void (*my_signal(int sig, void (*handler)(int)))(int) {
  struct sigaction sa_old, sa_new;
  sa_new.sa_handler = handler;
  sigemptyset(&sa_new.sa_mask);
#ifdef OLD_SIG
  sa_new.sa_flags = SA_RESTART | SA_NODEFER;
#else
  sa_new.sa_flags = SA_RESTART;
#endif

  if (sigaction(sig, &sa_new, &sa_old) == -1)
    return SIG_ERR;
  return sa_old.sa_handler;
}

static void handler(int sig) {
  printf("Caught a signal %d(%s)\n", sig, strsignal(sig));
  _exit(0);
}

int main() {
  printf("using my_signal\n");
  if (my_signal(SIGINT, handler) == SIG_ERR)
    err_sys("my_signal error");
  printf("Ctrl-C to test my_signal and exit\n");
  for (;;)
    pause();
  exit(0);
}