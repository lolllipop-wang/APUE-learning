/**
 * 测试SIG_SETMASK和SIG_BLOCK的功能有何不同
 *
 */
#define _GNU_SOURCE
#include "../../include/signal_functions.h"
#include <apue.h>

int main(int argc, char *argv[]) {
  sigset_t masks_set, masks_block;
  sigemptyset(&masks_set);
  sigemptyset(&masks_block);
  sigaddset(&masks_set, SIGINT);
  sigaddset(&masks_block, SIGQUIT);
  sigaddset(&masks_block, SIGSEGV);

  print_sig_mask(stdout, "signal masks before initialize:\n");

  if (sigprocmask(SIG_SETMASK, &masks_set, NULL) < 0)
    err_sys("sigprocmask error - SIG_SETMASK");

  print_sig_mask(stdout, "signal masks after SIG_SEKMASK:\n");

  if (sigprocmask(SIG_BLOCK, &masks_block, NULL) < 0)
    err_sys("sigprocmask error - SIG_BLOCK");

  print_sig_mask(stdout, "signal masks after SIG_BLOCK:\n");

  exit(0);
}