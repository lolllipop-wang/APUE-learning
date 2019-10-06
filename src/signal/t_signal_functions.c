#include "../../include/signal_functions.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  sigset_t sigset;
  sigemptyset(&sigset);
  sigfillset(&sigset);
  print_sigset(stdout, "Signal: ", &sigset);
  exit(EXIT_SUCCESS);
}