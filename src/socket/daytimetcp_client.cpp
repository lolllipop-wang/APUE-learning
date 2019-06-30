//TCP获取时间客户端程序
#include <apue.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int BUFFSIZE = 4096;

char buf[BUFFSIZE];

int main(int argc, char *argv[])
{
    int sockfd, nread;
    struct sockaddr_in  servaddr;

    if (argc != 2)
        err_quit("usage: %s <IP address>", argv[0]);
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");
    
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1998);
    
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_sys("inet_pton error");
    
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");
    

    while ((nread = read(sockfd, buf, BUFFSIZE)) > 0) {
        buf[nread] = 0;
        if (fputs(buf, stdout) == EOF)
            err_sys("fputs error");
    }

    if (nread < 0)
        err_sys("read error");
    close(sockfd);
    return 0;
}