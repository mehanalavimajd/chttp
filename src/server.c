#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#define PORT 8010
#define MAXCONN 20
#define LOCALHOST "127.0.0.1"
#define SA struct sockaddr
int main()
{
	int serverFD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddr, clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(LOCALHOST);
	int opt = 1; // Set to True so can Reuse Address.
	setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bind(serverFD, (SA*) &serverAddr, sizeof(serverAddr));
	listen(serverFD, MAXCONN);
	int i = 0;
	while (1)
	{
		int clientAddrSize = sizeof(clientAddr);
		int clientFD = accept(serverFD, (SA*) &clientAddr, &clientAddrSize);
		char buff[100000]; // 100 Bytes
		read(clientFD, buff, 99999);
		char *ret = "HTTP/1.1 201 Created\r\n\r\n";
		write(clientFD, ret, strlen(ret));
		printf("%s, %d\n", buff, i++);
		close(clientFD);
	}
	return 0;
}