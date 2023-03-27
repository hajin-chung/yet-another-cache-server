#include <stdint.h>

#define MAX_EVENT 100
#define MAX_CONN 10

int PORT = 8000;

int main(int argc, char *argv[]);
void createTcpSocket(int* sock, int port);
void handleNewClient(int epollfd, int serverSocket);
void handleRequest();
void epoll_ctl_add(int epollfd, int fd, uint32_t events);
int setNonBlocking(int fd);