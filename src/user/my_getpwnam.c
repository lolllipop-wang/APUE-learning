#include <pwd.h>
#include <apue.h>
#include <errno.h>

struct passwd *my_getpwnam(const char *username)
{
    if(username == NULL || *username == '\0')
        return NULL;
    
    struct passwd *pwd;
    while((pwd = getpwent()) != NULL) {
        if(strcmp(pwd -> pw_name, username) == 0)
            return pwd;
    }
    endpwent();
    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        err_quit("usage: %s username", argv[0]);
    
    struct passwd *pwd;
    pwd = my_getpwnam(argv[1]);

    if(pwd == NULL)
        err_sys("%s: No such user", argv[0]);
    
    printf("name: %s\n", pwd -> pw_name);
    printf("uid: %ld\n", (long)pwd -> pw_uid);
    printf("gid: %ld\n", (long)pwd -> pw_gid);
    printf("shell: %s\n", pwd -> pw_shell);
    printf("dir: %s\n", pwd -> pw_dir);

    exit(0);
}
