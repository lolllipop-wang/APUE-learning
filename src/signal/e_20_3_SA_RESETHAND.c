#include <apue.h>

static volatile sig_atomic_t sig_int = 0;

static void my_signal_handler(int sig) {
  if (sig == SIGINT) {
    psignal(sig, "Oh! I caught a signal");
    sig_int = 1;
  }
}

int main(int argc, char *argv[]) {
  struct sigaction action;
  action.sa_flags = SA_RESETHAND;
  sigemptyset(&action.sa_mask);
  action.sa_handler = my_signal_handler;

  printf("setting up signal handle for SIGINT\n");

  if (sigaction(SIGINT, &action, NULL) == -1)
    err_sys("sigaction error");
  printf("first waitting fro SIGINT\n");
  while (sig_int == 0)
    continue;

  sig_int = 0;

  printf("watting for SIGINT again\n");

  while (sig_int == 0)
    continue;
  exit(EXIT_SUCCESS);
}
