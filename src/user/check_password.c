/**
 * 验证用户的登陆名和密码
 */
#include <apue.h>
#include <crypt.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>

int main(int argc, char *argv[]) {
  int ok;             /* 登陆是否成功 */
  int lnmax;          /* 最大登陆名长度 */
  int length;         /* 输入登陆名的长度 */
  struct passwd *pwd; /* 从passwd文件中获取 */
  struct spwd *spwd;  /* 从shadow文件中获取 */
  char *username;     /* 输入的用户名 */
  char *password;     /* 输入的密码 */
  char *encrypted;    /* 输入的密码加密后的值 */

  lnmax = sysconf(_SC_LOGIN_NAME_MAX); /* 获取系统最大的登陆名长度限制 */

  if (lnmax < 0) /* 获取失败时假设为256 */
    lnmax = 256;

  username = (char *)malloc(lnmax);
  if (username == NULL)
    err_sys("%s: malloc error", argv[0]);

  printf("Username: ");
  fflush(stdout);
  if (fgets(username, lnmax, stdin) == NULL)
    err_sys("%s: fgets error", argv[0]);

  length = strlen(username);
  if (username[length - 1] == '\n')
    username[length - 1] = '\0';

  if ((pwd = getpwnam(username)) == NULL) /* 从passwd文件中查询输入的登陆名 */
    err_sys("%s: could't get passwd record", argv[0]);

  /* 尝试从shadow文件中获取用户密码，如果errno为EACCES，原因是访问权限不够，
      否则可能是因为密码存在与passwd文件中或者其他原因导致调用失败 */
  if ((spwd = getspnam(username)) == NULL && errno == EACCES)
    err_sys("%s: could't read shadow file", argv[0]);

  if (spwd != NULL)
    pwd->pw_passwd = spwd->sp_pwdp;

  password = getpass("Password: "); /* 输入密码， 屏蔽回显功能 */

  /* 加密新密码时会随机产生一个salt值，用于验证登陆时则从已加密密码中获取salt值
      salt字符串中只有前两个字符对crypt()函数有效 */
  encrypted = crypt(password, pwd->pw_passwd); /* 对输入的密码进行加密 */
  char *p;
  for (p = password; *p != '\0'; ++p) /* 清空内存，防止恶意攻击 */
    *p = 0;

  if (strcmp(encrypted, pwd->pw_passwd) == 0)
    ok = 1;
  else
    ok = 0;

  if (ok == 1)
    printf("Successed!\n");
  else
    printf("failed!\n");
  exit(0);
}