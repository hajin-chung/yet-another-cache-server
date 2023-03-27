#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"
#include "../src/logger.h"

int main(int argc, char *argv[]) {
	int clientSocket;
	struct sockaddr_in serverAddr;
	char buf[BUF_SIZE];

	if (argc >= 3) {
		PORT = atoi(argv[1]);
		ADDR = argv[2];
	}

	logger(INFO, "initializing client");
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		logger(ERROR, "socket creation");
		exit(0);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ADDR);
	serverAddr.sin_port = htons(PORT);

	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		logger(ERROR, "socket connection failed");
		exit(0);
	}

	memset(buf, '\0', sizeof(buf));
	printf("> ");
  scanf("%s", buf);
	write(clientSocket, buf, strlen(buf) - 1); // remove new line
  printf("length %ld\n", strlen(buf));

	memset(buf, '\0', sizeof(buf));	
  read(clientSocket, buf, BUF_SIZE);
	printf("< %s\n", buf);

	close(clientSocket);

	return 0;
}