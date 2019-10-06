/**
 * 使用character_filter做输入的过滤器。
 */
#include <apue.h>

int main(int argc, char *argv[]) {
  FILE *fin;
  char buf[MAXLINE];

  fin = popen("./character_filter", "r");
  if (fin == NULL)
    err_sys("%s: popen error", argv[0]);

  for (;;) {
    fputs("input> ", stdout);
    fflush(stdout);
    if (fgets(buf, MAXLINE, fin) == NULL)
      break;
    if (fputs(buf, stdout) == EOF)
      err_sys("%s: fputs error", argv[0]);
  }

  if (pclose(fin) == -1)
    err_sys("%s: pclose error", argv[0]);
  puts("\n");
  exit(0);
}