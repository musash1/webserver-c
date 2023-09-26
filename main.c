#include <stdio.h>
#include <netdb.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int connfd) {

}

int main() {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socker creation failed.\n");
    exit(0);
  }
  printf("socket successfully created.\n");
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    printf("socket bind failed.\n");
    exit(0);
  }
  printf("socket successfully binded.\n");

  if ((listen(sockfd, 5)) != 0) {
    printf("Listen failed.\n");
    exit(0);
  }
  printf("Server listening.\n");
  len = sizeof(cli);

  connfd = accept(sockfd, (SA*)&cli, &len);
  if (connfd < 0) {
    printf("server accep failed.\n");
    exit(0);
  }
  printf("server accept the client.\n");

  func(connfd);
  close(sockfd);
}
