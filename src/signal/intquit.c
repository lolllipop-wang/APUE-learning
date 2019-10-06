#include <apue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void my_signal_handler(int sig) {
  static int count = 0;

  if (sig == SIGINT) {
    count++;
    printf("caught SIGINT (%d)\n", count);
    return;
  }

  printf("caught SIGQUIT - that's all folks!\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  if (signal(SIGINT, my_signal_handler) == SIG_ERR)
    err_sys("signal error");
  if (signal(SIGQUIT, my_signal_handler) == SIG_ERR)
    err_sys("signal error");

  for (;;)
    pause();

  exit(EXIT_SUCCESS);
}