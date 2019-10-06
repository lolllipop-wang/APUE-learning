/**
 * 检查文件权限
 * ===================================================================
 * #include <unistd.h>
 *
 * int access(const char *pathname, int mode);
 * int faccessat(int fd, const char *pathname, int mode, int flag);
 *
 * Return 0 on success, or -1 on error.
 * ====================================================================
 * 以下两种情况access()和faccessat()函数相同
 *
 * 1. faccessat()函数的pathname为绝对路径。
 * 2. faccessat()函数fd的取值为AT_FDCWD，pathname为相对路径。
 */

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2)
    err(EXIT_FAILURE, "usage: %s <pathname>", argv[0]);

  if (access(argv[1], R_OK) < 0)
    err(EXIT_FAILURE, "read access error for %s", argv[1]);
  else
    printf("read access OK\n");

  if (access(argv[1], W_OK) < 0)
    err(EXIT_FAILURE, "write access error for %s", argv[1]);
  else
    printf("write access OK\n");

  if (access(argv[1], X_OK) < 0)
    err(EXIT_FAILURE, "execute access error for %s", argv[1]);
  else
    printf("execute access OK\n");

  if (open(argv[1], O_RDONLY) < 0)
    err(EXIT_FAILURE, "open error for %s", argv[1]);
  else
    printf("open for reading OK\n");
  exit(0);
}