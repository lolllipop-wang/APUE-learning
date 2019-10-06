/**
 *
 * 使用管道在父子进程之间传输数据。
 * =====================================
 * #include <unistd.h>
 *
 * int pipe(int pipefd[2]);
 *
 * Return 0 on success, or -1 on error.
 * =====================================
 *
 */
#include <apue.h>

char line[MAXLINE];

int main(int argc, char *argv[]) {
  pid_t pid;
  int fd[2];
  int nread;
  int nwrite;
  char buf[] = "Hello World!\n";

  if (pipe(fd) < 0)
    err_sys("%s: pipe error", argv[0]);

  switch ((pid = fork())) {
  case -1:
    err_sys("fork error");
    break;
  case 0:
    close(fd[0]);
    write(fd[1], buf, sizeof(buf));
    exit(0);
  default:
    break;
  }
  close(fd[1]);
  nread = read(fd[0], line, MAXLINE);
  if (nread < 0)
    err_sys("%s: read from pipe failed", argv[0]);
  nwrite = write(STDOUT_FILENO, line, nread);
  if (nwrite < 0)
    err_sys("%s: write error", argv[0]);
  else if (nwrite < nread)
    err_sys("%s: partial write", argv[0]);

  exit(0);
}