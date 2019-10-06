/**
 * 每次一页的显示已产生的输出
 *
 *     使用管道，将子进程的管道读端重定向到标准输入，每次父进程从文件中
 * 读入一行，并且写入管道写端，子进程（exec执行后替换为pager程序）从管道
 * 读端读入后显示。
 */

#include <apue.h>
#include <sys/wait.h>

#define DEF_PAGER                                                                               \
  "/usr/bin/more" /* 如果环境变量中不存在PAGER变量，则默认使用'/usr/bin/more' \
                   */

int main(int argc, char *argv[]) {
  FILE *fp;           /* 指向argv[1]表示的文件的文件指针 */
  pid_t pid;          /* 子进程PID */
  int fd[2];          /* 用于管道的文件描述符 */
  int status;         /* 子进程的退出状态 */
  int length, nwrite; /* 每行读入的长度以及写的字节数 */
  char *pager,
      *argv0; /* 分别指向pager程序和传递给pager程序的第一个参数（pager名称） */
  char buf[MAXLINE]; /* 用于存储每次读入的一行 */

  if (argc != 2)
    err_quit("usage: %s file", argv[0]);

  fp = fopen(argv[1], "r"); /* 打开文件 */
  if (fp == NULL)
    err_sys("%s: open file error", argv[0]);

  if (pipe(fd) < 0) /* 创建管道 */
    err_sys("%s: pipe error", argv[0]);

  switch ((pid = fork())) {
  case -1:
    err_sys("fork error");
  case 0:
    close(fd[1]); /* 关闭管道写端 */

    if (fd[0] != STDIN_FILENO) { /* 将管道读端重定向到标准输入 */
      if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
        err_sys("%s: dup2 fd error to stdin", argv[0]);

      close(fd[0]); /* 此时fd[0]已经没用了 */
    }

    if ((pager = getenv("PAGER")) ==
        NULL)            /* 尝试从环境变量中获取PAGER变量的值 */
      pager = DEF_PAGER; /* 如果获取失败就使用默认的'/usr/bin/more' */

    if ((argv0 = strrchr(pager, '/')) !=
        NULL)  /* 检测pager字符串最右端是否有'/' */
      argv0++; /* 有则跳过 */
    else
      argv0 = pager; /* 最右端没有'/' */

    if (execlp(pager, argv0, (char *)NULL) < 0) /* 执行pager程序 */
      err_sys("%s: can't execute %s", argv[0], pager);

  default:
    break;
  }

  close(fd[0]); /* 关闭管道读端 */

  while (fgets(buf, MAXLINE, fp) != NULL) {
    length = strlen(buf);
    nwrite = write(fd[1], buf, length); /* 将读入的数据写入管道写端 */

    if (nwrite < 0)
      err_sys("%s: write error", argv[0]);
    else if (nwrite < length)
      err_sys("%s: partial write", argv[0]);
  }

  if (ferror(fp))
    err_sys("%s: fgets error", argv[0]);

  close(fd[1]);

  if (waitpid(pid, &status, 0) < 0)
    err_sys("waitpid error");

  pr_exit(status);
  exit(0);
}