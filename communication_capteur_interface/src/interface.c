#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h> 
#include <sys/stat.h> 
#define SA struct sockaddr 
void function(int sockfd) 
{
	 char buffer[BUFSIZ];
	 char *file_name = "info_recv.txt";
         int filefd; 
	 ssize_t read_return;
          printf("Veuillez saisir le message à envoyer :");
	  scanf("%s",buffer);
	  send(sockfd,buffer,sizeof(buffer),0);
	  bzero(buffer,sizeof(buffer));
	filefd = open(file_name,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (filefd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        do {
            read_return = read(sockfd, buffer, BUFSIZ);
		printf("%s",buffer); 
		bzero(buffer,sizeof(buffer));
		
            if (read_return == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (write(filefd, buffer, read_return) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        } while (read_return > 0);
		
              close(filefd);

} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	//  create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 
        // define ip adress + port
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(8000); 

	// client connect 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// receive informations 
	function(sockfd); 
	close(sockfd); 
} 

