#define _POSIX_C_SOURCE 1
#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> /* for fprintf */ 
#include <string.h> /* for memcpy */ 
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include "talk.h"
int main(int argc, char* argv[])
	{
        if (argc < 3) {
            fprintf(stderr, "Usage: %s IP_Adrress PORT\n", argv[0]);
            return EXIT_FAILURE;
        }
        
    struct addrinfo hints; 
    struct addrinfo *servaddr;
    int status;
    int socketfd;
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype= SOCK_STREAM;
	char *ip=argv[1];
	char *port=argv[2];
	if ((status = getaddrinfo(ip,port, &hints, &servaddr)) != 0){
        
    	fprintf(stderr, "getaddrinfo error:%s\n", gai_strerror(status));
    	exit(1);
    	}
	socketfd=socket(servaddr->ai_family,servaddr->ai_socktype,servaddr->ai_protocol);
	if (connect(socketfd, servaddr->ai_addr, servaddr->ai_addrlen) < 0) { 
	    perror("connect failed");
    	return 0; 
    }
    
    int result = cs428_talk(STDIN_FILENO, socketfd, STDOUT_FILENO);
}
