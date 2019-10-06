#define _GNU_SOURCE
#include "../../include/signal_functions.h"
#include <apue.h>
#include <errno.h>
#include <time.h>

static volatile sig_atomic_t get_sigquit = 0;

static void handler(int sig) {
  printf("caught signal: %d(%s)\n", sig, strsignal(sig));

  if (sig == SIGQUIT)
    get_sigquit = 1;
}

int main(int argc, char *argv[]) {
  int loop_count;
  time_t start_time;
  sigset_t blocking_mask, old_mask;
  struct sigaction sa;

  print_sig_mask(stdout, "Initial mask is:\n");

  sigemptyset(&blocking_mask);
  sigaddset(&blocking_mask, SIGQUIT);
  sigaddset(&blocking_mask, SIGINT);

  if (sigprocmask(SIG_SETMASK, &blocking_mask, &old_mask) < 0)
    err_sys("sigprocmask error");

  sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;
  sa.sa_flags = 0;
  if (sigaction(SIGINT, &sa, NULL) < 0)
    err_sys("setting up SIGINT handler error");
  if (sigaction(SIGQUIT, &sa, NULL) < 0)
    err_sys("setting up SIGQUIT handler error");

  for (loop_count = 1; get_sigquit == 0; ++loop_count) {
    printf("\n\n=== Loop: %d\n", loop_count);

    print_sig_mask(stdout, "start critical, signal mask is:\n");

    for (start_time = time(NULL); time(NULL) < start_time + 4;)
      continue;

    print_pending_sigs(stdout, "before sigsuspend, pending signals:\n");

    if (sigsuspend(&old_mask) < 0 && errno != EINTR)
      err_sys("sigsuspend error");
  }

  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    err_sys("sigprocmask error");

  print_sig_mask(stdout, "===Loop exited\nrestore signal mask to:\n");

  exit(0);
}