#include <apue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  sigset_t block_set, pre_mask;

  sigemptyset(&block_set);
  sigaddset(&block_set, SIGINT);
  if (sigprocmask(SIG_BLOCK, &block_set, &pre_mask) == -1)
    err_sys("sigprocmask error");
  /**
   * code begin
   */

  /**
   * code end
   */

  if (sigprocmask(SIG_SETMASK, &pre_mask, NULL) == -1)
    err_sys("sigpromask error");

  exit(EXIT_SUCCESS);
}
