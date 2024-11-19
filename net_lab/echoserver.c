#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define LISTENQ 1024
#define MAXLINE 1024

int main(int argc, char **argv) 
{
	int listenfd, connfd, port, clientlen;
  struct sockaddr_in serveraddr, clientaddr;
	char buf[MAXLINE];

	//Create a socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
		perror("socket");
		exit(1);
	}
	
	//Associate the socket with a pair of address and port number
	port = atoi(argv[1]);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if (bind(listenfd, (struct socketaddr*)&serveraddr, sizeof(serveraddr)) == -1 ){
		perror("Failed ro bind. ");
		return -1;
	}

	//Start waiting
	if (listen(listenfd, LISTENQ) < 0)
		return -1;
	
	//Wait for connetion request
	printf("Waiting for request... \n");

	clientlen = sizeof(clientaddr);
	if ((connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen)) < 0 ){
		perror("accept");
		exit(-1);
	}

	char* caddrp = inet_ntoa(clientaddr.sin_addr);
	printf("Connected to %s. \n", caddrp);

	int n;
	while (1){
		n = read(connfd, buf, MAXLINE);
		if (n == 0 ) break;
		printf("Server received %d bytes. \n", n);
		write(connfd, buf, MAXLINE);
	}
	return 0;
}
