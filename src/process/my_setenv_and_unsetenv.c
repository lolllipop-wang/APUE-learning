#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

int my_setenv(const char *name, const char *value, int overwrite);
int my_unsetenv(const char *name);

int
main(int argc, char *argv[])
{
    /* 
    方便调试
    if(clearenv() != 0) {
        perror("clearenv error");
        exit(EXIT_FAILURE);
    }
    */
    if(my_setenv("SCHOOL", "XAUT", 1) == 0) {
        printf("my_setenv successed!\n");
    }
    else {
        printf("my_setenv failed\n");
        exit(EXIT_FAILURE);
    }

    if(my_unsetenv("SCHOOL") == 0)
        printf("my_unsetenv successed!\n");
    else {
        printf("my_unsetenv error\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int
my_setenv(const char *name, const char *value, int overwrite)
{
    if(name == NULL || value == NULL || (overwrite != 0 && overwrite != 1))
        return -1;
    char *new_env, *q;
    const char *p;
    int name_length  = strlen(name);
    int value_length = strlen(value);
    int i;
    /* 为新的环境变量申请空间 */
    new_env = (char *)malloc(sizeof(char) * (name_length + value_length + 2));

    if(new_env == NULL) {
        return -1;
    } 
    /* 将name, value中的内容复制到新的空间中，中间使用'='分隔 */
    /* Copy begin */
    p = name;
    q = new_env;
    while(*p != '\0')
        *q++ = *p++;
    *q++ = '=';

    p = value;
    while(*p != '\0')
        *q++ = *p++;
    *q = 0;
    /* Copy end */

    /* 如果environ为NULL，则直接加入新的环境变量 */
    if(environ == NULL || *environ == NULL) {
        return putenv(new_env);
    }


    if(overwrite == 0) {
        /* overwrite 为 0 的情况 */
        char **ep;
        char *env_ptr = NULL;

        /* 在环境变量中寻找要插入的变量是否已经存在 */
        for(ep = environ; *ep != NULL; ++ep) {
            if(strcmp(*ep, new_env) == 0) {
                env_ptr = *ep;
                break;
            }
        }

        /* 不存在则加入， 存在直接返回 */
        if(env_ptr == NULL)
            return putenv(new_env);
        else 
            return 0;
    }
    else {
        /* overwrite 为 1 的情况 */
        char **ep;
        char *env_ptr = NULL;

        /* 查询新加入的变量是否已经存在， 如果存在就是用新的变量替换 */
        for(ep = environ; *ep != NULL; ++ep) {
            if(strcmp(*ep, new_env) == 0) {
                *ep = new_env;
                env_ptr = *ep;
                break;
            }
        }
        /* 不存在就加入 */
        if(env_ptr == NULL)
            return putenv(new_env);
        else 
            return 0;
    } 
    return -1;
}

int 
my_unsetenv(const char *name)
{
    if(name == NULL) {
        return -1;
    }
    int flag = 0;
    char **ep;
    char *env_ptr;
    char buf[1000];

    /* 在环境变量中寻找要删除的变量 */
    for(ep = environ; *ep != NULL; ++ep) {
        env_ptr = *ep;
        char *p = buf;
        char *q = env_ptr;
        
        /* 获取当前这一变量的 name */
        while(q != NULL && *q != '=')
            *p++ = *q++;
        *p = 0;

        /* 判断当前变量是否为要删除的变量 */
        if(strcmp(buf, name) == 0) {
            flag = 1;
            char **ep2 = ep + 1;

            /* 将后面的变量向前移动 */
            while(*ep2 != NULL) {
                *(ep2 - 1) = *ep2;
                ep2++;
            }
            *(ep2 - 1) = NULL;
            /* 删除当前变量 */
            free(env_ptr);
        }

    }

    if(flag == 0)
        return -1;

    return 0;
}