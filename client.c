#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> /* for fprintf */ 
#include <string.h> /* for memcpy */ 
#include <netdb.h>
#in
int main()
	{
struct addrinfo hints; 
struct addrinfo *servaddr;
int status;
int socketfd;
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype= SOCK_STREAM;
	char *ip='10.33.1.19';
	char *port='8080';
	if ((status = getaddrinfo(ip,port, &hints, &servaddr)) != 0)
	 {
	fprintf(stderr, "getaddrinfo error:%s\n", gai_strerror(status));
	exit(1);
	}
	socketfd=socket(servaddr->ai_family,servaddr->ai_socktype,servaddr->ai_protocol);
	if (connect(socketfd, servaddr->ai_addr, servaddr->ai_addrlen)) < 0) { 
	perror("connect failed");
	return 0; }
}
