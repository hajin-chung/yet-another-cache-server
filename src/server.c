#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <fcntl.h> 

#include "server.h"
#include "logger.h"
#include "request.h"
#include "response.h"

int main(int argc, char *argv[]) {
	struct epoll_event events[MAX_EVENT];
	int serverSocket, clientSocket, nfds, epollfd;
	int i;

	if (argc >= 2) {
		PORT = atoi(argv[1]);
	}

	logger(INFO, "initializing PORT = %d", PORT);

	// initializing epoll
	epollfd = epoll_create1(0);
	if (epollfd == -1) {
		logger(ERROR, "epoll creation error on line 25");
		exit(0);
	}

	// initializing server socket
	createTcpSocket(&serverSocket, PORT); 

	epoll_ctl_add(epollfd, serverSocket, EPOLLIN);

	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENT, -1);
		if (nfds == -1) {
			logger(ERROR, "epoll_wait");
			exit(0);
		}

		for (i = 0; i < nfds; i++) {
			if (events[i].data.fd == serverSocket) { 
				// new connection
				handleNewClient(epollfd, serverSocket);
			} else if (events[i].events & EPOLLIN) { 
				// handle client request
				handleRequest(epollfd, events[i].data.fd);

				// close connection right after request
				// close(events[i].data.fd);
				// epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
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

	if (bind(*sock, (struct sockaddr*) &addr, sizeof(addr))==-1) {
		logger(ERROR, "TCP socket bind() error");
		exit(0);
	}

	setNonBlocking(*sock);
	if (listen(*sock, MAX_CONN) == -1) {
		logger(ERROR, "TCP socket listen() error");
		exit(0);
	}
}

void handleNewClient(int epollfd, int serverSocket) {
	int clientSocket, clientLen;
	struct sockaddr_in clientAddr;

	clientLen = sizeof(clientAddr);
	clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

	setNonBlocking(clientSocket);
	epoll_ctl_add(epollfd, clientSocket, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
}

void handleRequest(int epollfd, int clientSocket) {
	char buf[BUF_SIZE];
	int n;

	memset(buf, 0, sizeof(buf));
  // TODO: handle requests bigger than BUF_SIZE
	n = read(clientSocket, buf, sizeof(buf)); 

  struct Query* query = parseRequest(buf); 
  if (query->isError) {
    const char* message = "something gone wrong";
    char* response = encodeError(0, strlen(message), message);
    write(clientSocket, response, sizeof(response));
  } else if (query->type == SetQueryType) {
    // TODO: implement
  } else if (query->type == GetQueryType) {
    // TODO: implement
  } else if (query->type == DelQueryType) {
    // TODO: implement
  } else {
    const char* message = "something gone wrong";
    char* response = encodeError(0, strlen(message), message);
    write(clientSocket, response, sizeof(response));
  }

  free(query);
}

void epoll_ctl_add(int epollfd, int fd, uint32_t events) {
	struct epoll_event ev;

	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		logger(ERROR, "epoll_ctl adding fd");
		exit(0);
	}
}

int setNonBlocking(int fd) { // TODO: understand this
	if (fcntl(fd, F_SETFD, fcntl(fd, F_GETFD, 0) | O_NONBLOCK) == -1) {
		return -1;
	}
	return 0;
}