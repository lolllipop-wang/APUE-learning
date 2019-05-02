#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pwd.h>
#include <grp.h>
#include <stdio.h>

/**
 * 通过uid获取用户名
 */
char *username_by_uid(uid_t uid)
{
    struct passwd *pwd;

    pwd = getpwuid(uid);

    if(pwd == NULL)
        return NULL;
    
    return pwd -> pw_name;
}

/**
 * 通过用户名获取uid
 */
uid_t uid_by_username(char *username)
{
    if(username == NULL || *username == '\0')
        return -1;
    
    uid_t uid;
    struct passwd *pwd;

    pwd = getpwnam(username);

    if(pwd == NULL)
        return -1;
    
    return pwd -> pw_uid;
}

/**
 * 通过gid获取组名
 */
char *groupname_by_gid(gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);

    if(grp == NULL)
        return NULL;

    return grp -> gr_name;
}

/**
 * 通过组名获取gid
 */
gid_t gid_by_groupname(char *groupname)
{
    struct group *grp;

    grp = getgrnam(groupname);

    if(grp == NULL)
        return NULL;
    
    return grp -> gr_gid;
}

#ifdef __cplusplus
}
#endif

#endif /* USER_FUNCTION_H */