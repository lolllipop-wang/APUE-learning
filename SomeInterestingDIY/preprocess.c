#include <apue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINESIZE 4096

char line[MAXLINESIZE];

int main(int argc, char *argv[]) {
  if (argc != 3)
    err_quit("usage: %s <in-pathname> <out-pathname>", argv[0]);

  int i;
  int nu;          /* 源文件中的行号 */
  int length;      /* 每行的长度 */
  int count;       /* 注释的层数 */
  int line_ch;     /* 每行已输出的字符数量 */
  int flag, cando; /* 标记是否在注释中 */
  FILE *ifp, *ofp; /* 源文件和输出文件的文件指针 */

  if ((ifp = fopen(argv[1], "r")) == NULL)
    err_sys("can't open source file");

  if ((ofp = fopen(argv[2], "w")) == NULL)
    err_sys("can't open output file");

  nu = 0;
  count = 0;

  flag = 0;
  cando = 1;

newline:
  while (fgets(line, MAXLINESIZE, ifp) != NULL) {
    ++nu;

    int length = strlen(line);

    if (line[length - 1] == '\n') {
      line[length - 1] = '\0';
      length--;
    }

    i = 0;
    while (i < length && line[i] == ' ' ||
           line[i] == '\n') /* 跳过每一行的空白字符 */
      i++;

    line_ch = 0;
    for (; i < length; ++i) {
      /* 遇到 "/" "*" 注释开始*/
      if (i + 1 < length && line[i] == '/' && line[i + 1] == '*') {
        flag = 1;
        cando = 0;
        count++;
        i += 1;

        /* 出现嵌套的"/" "*" */
        if (count > 1) {
          fprintf(stderr, "error at line %d: %s\n", nu, line);
          exit(1);
        }

        /* 遇到 "*" "/" 注释结束 */
      } else if (i + 1 < length && line[i] == '*' && line[i + 1] == '/') {
        flag = 0;
        cando = 1;
        count--;
        i += 2;

        /* 出现嵌套的"/" "*" */
        if (count < 0) {
          fprintf(stderr, "error at line %d: %s\n", nu, line);
          exit(1);
        }

        /* 单行注释 */
      } else if (i + 1 < length && line[i] == '/' && line[i + 1] == '/') {
        goto newline;
      }

      /* 当前字符是否可以输出 */
      if (flag == 0 && cando == 1 && i < length) {
        fputc(line[i], ofp);
        line_ch++;
      }
    }

    /* 如果当前行没有可输出的字符，则不输出'\n' 避免多余空行 */
    if (line_ch > 0)
      fputc('\n', ofp);
  }

  fclose(ifp);
  fclose(ofp);

  printf("%s: done!\n", argv[0]);

  exit(0);
}