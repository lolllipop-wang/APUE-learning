// TCP获取时间客户端程序
#include <arpa/inet.h>
#include <err.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUFFSIZE = 4096;

char buf[BUFFSIZE];

int main(int argc, char *argv[]) {
  int sockfd, nread;
  struct sockaddr_in servaddr;

  if (argc != 2)
    err(EXIT_FAILURE, "usage: %s <IP address>", argv[0]);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err(EXIT_FAILURE, "socket error");

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(1998);

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    err(EXIT_FAILURE, "inet_pton error");

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    err(EXIT_FAILURE, "connect error");

  while ((nread = read(sockfd, buf, BUFFSIZE)) > 0) {
    buf[nread] = 0;
    if (fputs(buf, stdout) == EOF)
      err(EXIT_FAILURE, "fputs error");
  }

  if (nread < 0)
    err(EXIT_FAILURE, "read error");
  close(sockfd);
  return 0;
}