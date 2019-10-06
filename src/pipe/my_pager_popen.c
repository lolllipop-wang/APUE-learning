/**
 * 使用popen和pclose函数重写my_pager程序
 * ======================================================
 * #include <stdio.h>
 *
 * FILE *popen(const char *cmd, const char *type);
 *
 * Return file pointer on success, or NULL on error.
 *
 *              type = "r"
 *      parent(fp)<-----(stdout)child
 *
 *              type = "w"
 *      parent(fp)----->(stdin)child
 *
 * int pclose(FILE *fp);
 *
 * Return cmd exited status on success, or -1 on error.
 * ======================================================
 */
#include <apue.h>
#include <sys/wait.h>

#define PAGER                                                                            \
  "${PAGER:-more}" /* 如果环境变量里有PAGER,且其值非空，则使用其值， \
                      否则用more */

int main(int argc, char *argv[]) {
  char buf[MAXLINE];
  FILE *fin, *fout;

  if (argc != 2)
    err_quit("usage: %s file", argv[0]);

  fin = fopen(argv[1], "r");
  if (fin == NULL)
    err_sys("%s: can't open file %s", argv[0], argv[1]);

  fout = popen(PAGER, "w"); /* sh -c PAGER */
  if (fout == NULL)
    err_sys("%s: popen error", argv[0]);

  while (fgets(buf, MAXLINE, fin) != NULL) {
    if (fputs(buf, fout) == EOF)
      err_sys("%s: fputs error", argv[0]);
  }

  if (ferror(fin))
    err_sys("%s: fgets error", argv[0]);

  if (pclose(fout) == -1)
    err_sys("%s: pclose error", argv[0]);
  exit(0);
}