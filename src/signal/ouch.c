#include <apue.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void my_signal_handler(int sig) {
  printf("啪！你死了\n");
  exit(0);
}

int main(int argc, char *argv[]) {
  if (signal(SIGINT, my_signal_handler) == SIG_ERR)
    err_sys("signal error");

  int j;
  for (j = 0; j < INT_MAX; ++j) {
    printf("%d\n", j);
  }
  return 0;
}