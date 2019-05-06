/**
 * 使用uname()获取系统标识
 * =======================================
 * #include <sys/utsname.h>
 * 
 * int uname(struct utsname *utsbuf);
 * 
 * Return 0 on success, or -1 on error
 * =======================================
 */
 #include <apue.h>
 #include <sys/utsname.h>

 int main(int argc, char *argv[])
 {
     struct utsname name;

     if (uname(&name) < 0)
        err_sys("uname error");
    
     printf("System name: %s\n", name.sysname);
     printf("Node name:   %s\n", name.nodename);
     printf("Release:     %s\n", name.release);
     printf("Version:     %s\n", name.version);
     printf("Machine:     %s\n", name.machine);
     
#if defined(_GNU_SOURCE)
    printf("Domain name:  %s\n", name.domainname);
#endif
     exit(0);
 }