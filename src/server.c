#include <sys/socket.h>
#include <stdio.h>		
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
int main(){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct in_addr localhost; 
	inet_aton("127.0.0.1", &localhost); // localhost addr should be bytes not char *
	struct sockaddr_in addr = {AF_INET, htons(8080), localhost}; // htons(8080) turns it into bytes	
	char *opt = "w";
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR , opt, sizeof(opt));
	bind(fd, &addr, sizeof(addr));
	while(1){
		write(fd, "hello", 6);
		sleep(1);
		printf("S");
		fflush(stdout);
	}
	return 0;
}
