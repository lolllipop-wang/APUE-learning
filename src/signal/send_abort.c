#include <apue.h>
#include <signal.h>

int main(int argc, char *argv[]) {
  if (argc != 2)
    err_quit("usage: %s PID", argv[0]);

  pid_t pid = atol(argv[1]);

  if (kill(pid, SIGABRT) == -1)
    err_sys("send signal SIGABRT error");
  exit(0);
}