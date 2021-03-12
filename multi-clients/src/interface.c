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

void function(int sock) 
{
	char buffer[BUFSIZ];
	 char *file_name = "info_recv.txt";
         int filefd; 
	 ssize_t read_return;
          printf("Veuillez saisir le message à envoyer :");
	  scanf("%s",buffer);
	  send(sock,buffer,sizeof(buffer),0);
	  bzero(buffer,sizeof(buffer));
	filefd = open(file_name,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (filefd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        do {
            read_return = read(sock, buffer, BUFSIZ);
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

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;

	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8000 );

	//Connect to remote server
	if (connect(sock , (SA*)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
     function(sock);
	
	
	close(sock);
	return 0;
}
