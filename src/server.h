#define MAX_EVENT 100

int PORT = 8000;

int main(int argc, char *argv[]);
void createTcpSocket(int* sock, int port);
void handleNewClient(int epollfd, int serverSocket);