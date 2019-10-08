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
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 4096

#define PAGER  "${PAGER:-more}" /* 如果环境变量里有PAGER,且其值非空，则使用其值. 否则用more */

int main(int argc, char *argv[]) {
    char buf[MAXLINE];
    FILE *fin, *fout;

    if (argc != 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        exit(1);
    }

    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        fprintf(stderr, "%s: can't open file %s\n", argv[0], argv[1]);
        exit(1);
    }

    fout = popen(PAGER, "w"); /* sh -c PAGER */
    if (fout == NULL) {
        fprintf(stderr, "%s: popen error\n", argv[0]);
        exit(1);
    }

    while (fgets(buf, MAXLINE, fin) != NULL) {
        if (fputs(buf, fout) == EOF) {
            fprintf(stderr, "%s: fputs error\n", argv[0]);
            exit(1);
        }
    }

    if (ferror(fin)) {
        fprintf(stderr, "%s: fgets error\n", argv[0]);
        exit(1);
    }

    if (pclose(fout) == -1) {
        fprintf(stderr, "%s: pclose error\n", argv[0]);
    }

    exit(0);
}
