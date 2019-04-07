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
    if(clearenv() != 0) {
        perror("clearenv error");
        exit(EXIT_FAILURE);
    }
    if(my_setenv("SCHOOL", "XAUT", 1) == 0) {
        printf("my_setenv successed!\n");
    }
    else {
        printf("my_setenv failed\n");
        exit(EXIT_FAILURE);
    }

    if(my_unsetenv("SCHOOL") == 0)
        printf("my_unsetenv successed\n");
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

    new_env = (char *)malloc(sizeof(char) * (name_length + value_length + 2));

    if(new_env == NULL) {
        return -1;
    } 
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
    if(environ == NULL || *environ == NULL) {
        return putenv(new_env);
    }
    if(overwrite == 0) {
        char **ep;
        char *env_ptr = NULL;
        for(ep = environ; *ep != NULL; ++ep) {
            if(strcmp(*ep, new_env) == 0) {
                env_ptr = *ep;
                break;
            }
        }

        if(env_ptr == NULL)
            return putenv(new_env);
        else 
            return 0;
    }
    else {
        char **ep;
        char *env_ptr = NULL;
        for(ep = environ; *ep != NULL; ++ep) {
            if(strcmp(*ep, new_env) == 0) {
                *ep = new_env;
                env_ptr = *ep;
                break;
            }
        }
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
    for(ep = environ; *ep != NULL; ++ep) {
        env_ptr = *ep;
        char *p = buf;
        char *q = env_ptr;
        while(*q != '=')
            *p++ = *q++;
        *p = 0;

        if(strcmp(buf, name) == 0) {
            flag = 1;
            char **ep2 = ep + 1;
            while(*ep2 != NULL) {
                *(ep2 - 1) = *ep2;
                ep2++;
            }
        }
        free(env_ptr);
        ep++;

    }

    if(flag == 1) 
        return 0;
    return -1;
}