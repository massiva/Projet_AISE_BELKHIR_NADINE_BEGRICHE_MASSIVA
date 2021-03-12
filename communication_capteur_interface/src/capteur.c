#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h> 
#include <sys/stat.h> 
#define SA struct sockaddr 
// function send_file 
// Function designed for chat between client and server. 
void function(int sockfd) 
{ 
  
  int fp;
   char *file_name = "info.txt";	
   ssize_t read_return;
   char buffer[BUFSIZ];
   recv(sockfd,buffer,sizeof(buffer),0);
   printf("Message recu : %s \n",buffer);
   bzero(buffer,BUFSIZ);
    fp=open(file_name, O_RDONLY);
 if (fp == -1) {
        perror("open");
        exit(1);
    }
			
 while (1) {
        read_return = read(fp, buffer, BUFSIZ);
        if (read_return == 0)
            break;
        if (read_return == -1) {
            perror("read");
            exit(1);
        }
        if (write(sockfd, buffer, read_return) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
			
        
} 


int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Création de la socket..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(8000); 

	// Binding
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("Bind échoué...\n"); 
		exit(0); 
	} 
	else
		printf("Bind réussi..\n"); 

	// listening
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen échoué..\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept client
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("Client Accepté...\n"); 
        //send informations
	function(connfd); 
	close(sockfd); 
} 

