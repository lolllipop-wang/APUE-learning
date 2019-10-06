#define _GNU_SOURCE
#include "../../include/signal_functions.h"
#include <apue.h>

static void handler(int sig) {
  int x;
  printf("Caught a signal %d(%s)\n", sig, strsignal(sig));
  printf("Top of handler stack near    %10p\n", (void *)&x);

  _exit(0);
}

static void overflow(int call_num) {
  char a[100000];
  printf("Call %4d - top of stack near %10p\n", call_num, a);
  overflow(call_num + 1);
}

int main(int argc, char *argv[]) {
  stack_t sigstack;
  struct sigaction sa;
  int j;
  printf("Top of standard stack is near %10p\n", (void *)&j);

  sigstack.ss_sp = malloc(SIGSTKSZ);

  if (sigstack.ss_sp == NULL)
    err_sys("malloc error");
  sigstack.ss_size = SIGSTKSZ;
  sigstack.ss_flags = 0;

  if (sigaltstack(&sigstack, NULL) == -1)
    err_sys("signaltstack error");

  printf("Alternate stack is at       %10p - %p\n", sigstack.ss_sp,
         (void *)sbrk(0) - 1);

  sa.sa_handler = handler;
  sa.sa_flags = SA_ONSTACK;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGSEGV, &sa, NULL) == -1)
    err_sys("sigaction SIGSEGV error");
  overflow(1);

  exit(0);
}