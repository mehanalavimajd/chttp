/* client.c: A simple client used to 
 * just make sure that server is working
 * until it reaches to a level that can
 * return http.
 * This code won't be used in the end of
 * project.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
int main(){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct in_addr localhost; 
	inet_aton("127.0.0.1", &localhost); // localhost addr should be bytes not char *
	struct sockaddr_in mySocket = {AF_INET, htons(8080), localhost}; // htons(8080) turns it into bytes 
	connect(fd, &mySocket, sizeof(mySocket));
	while(1){
		sleep(1);
		char *buff;
		read(fd, buff, 10);
		printf("%s",buff);
	}
}
