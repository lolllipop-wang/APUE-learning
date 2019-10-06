#include <apue.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int result, sig;
  pid_t pid;

  if (argc != 3)
    err_quit("usage: %s, sig_num, pid", argv[0]);

  sig = atoi(argv[1]);
  pid = atol(argv[2]);

  result = kill(pid, sig);

  if (sig == 0) {
    if (result != 0)
      err_sys("kill error");
  } else {
    if (result == 0)
      printf("process exists and we can send it a signal!\n");
    else {
      if (errno == EPERM)
        printf("process exists but we don't have permission to send it a "
               "signal\n");
      else if (errno == ESRCH)
        printf("process does not exist\n");
      else
        err_sys("we are in big trouble\n");
    }
  }
  exit(EXIT_SUCCESS);
}