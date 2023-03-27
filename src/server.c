#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"
#include "logger.h"

int main(int argc, char *argv[]) {
  struct epoll_event ev, events[MAX_EVENT];
  int serverSocket, clientSocket, nfds, epollfd;
  int i;

  if (argc >= 2) {
    PORT = atoi(argv[1]);
  }

  logger(INFO, "initializing PORT = %d", PORT);

  // initializing server socket
  createTcpSocket(serverSocket, PORT); 

  // initializing epoll
  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    logger(ERROR, "epoll creation error on line 25");
    exit(0);
  }

  ev.events = EPOLLIN;
  ev.data.fd = serverSocket;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serverSocket, &ev) == -1) {
    logger(ERROR, "epoll_ctl adding serverSocket");
    exit(0);
  }

  for (;;) {
    nfds = epoll_wait(epollfd, events, MAX_EVENT, -1);
    if (nfds == -1) {
      logger(ERROR, "epoll_wait");
      exit(0);
    }

    for (i = 0; i < nfds; i++) {
      if (events[i].data.fd == serverSocket) { // new connection
        handleNewClient(epollfd, serverSocket);
      }
    }
  }

  return 0;
}

void createTcpSocket(int* sock, int port) {
  struct sockaddr_in addr;
  int on = 1;
  
  logger(INFO, "initializing socket");
	*sock = socket(PF_INET, SOCK_STREAM, 0);
	if (*sock == -1) {
    logger(ERROR, "TCP socket creation error");
    exit(0);
  }

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
	
	if (bind(*sock, (struct sockaddr*) &addr, sizeof(addr))==-1) {
    logger(ERROR, "TCP socket bind() error");
    exit(0);
	}

	if (listen(*sock, 100)==-1) {
    logger(ERROR, "TCP socket listen() error");
    exit(0);
	}
}

void handleNewClient(int epollfd, int serverSocket) {
}