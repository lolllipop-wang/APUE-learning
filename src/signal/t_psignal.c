#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s sig_num1 sig_num2...\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int sig_num, i;
  char *msg = "This is a massage";
  for (i = 1; i < argc; ++i) {
    sig_num = atoi(argv[i]);
    psignal(sig_num, msg);
  }
  exit(EXIT_SUCCESS);
}