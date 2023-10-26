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

enum HttpRequestType { GET, POST, PUT, DELETE };

const static struct {
  enum HttpRequestType val;
  const char *str;
} conversion [] = {
  {GET, "GET"},
  {POST, "POST"},
  {PUT, "PUT"},
  {DELETE, "DELETE"}
};

struct Message {
  int messageId;
  char *str;
};

struct Message getMessages() {
  struct Message message;
  message.messageId = 1;
  message.str = "Hello, Man";
  return message;  
}

struct Message parseRequest(char* buff) {
  char *method = strtok(buff, " ");
  enum HttpRequestType req;
  for (int i = 0; i < sizeof(conversion) / sizeof(conversion[0]); i++) {
    if (!strcmp(method, conversion[i].str)) {
      req = conversion[i].val;
    }
  }
  struct Message returnMethod;
  switch (req) {
    case GET:
      returnMethod = getMessages();
      break;
    default: break;
  }
  return returnMethod;
}

void func(int connfd) {
  char buff[MAX];
  int n;

  for (;;) {
    bzero(buff, MAX);

    read(connfd, buff, sizeof(buff));
    struct Message message = parseRequest(buff);
    
    printf("%s", message.str);
    char *header = "HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 55\r\nAccept-Ranges: bytes\r\nConnection: close\r\n\r\n<html><head></head><body>Hello World!</body></html>\r\n";
    send(connfd, header, strlen(header), 0);
    bzero(buff, MAX);
    n = 0;
    while ((buff[n++] = getchar()) != '\n');
    write(connfd, buff, sizeof(buff));

    if (strncmp("exit", buff, 4) == 0) {
      printf("Server Exit...\n");
      break;
    }
  }
}

int main() {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed.\n");
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
    printf("server accept failed.\n");
    exit(0);
  }
  printf("server accept the client.\n");

  func(connfd);
  close(sockfd);
}
