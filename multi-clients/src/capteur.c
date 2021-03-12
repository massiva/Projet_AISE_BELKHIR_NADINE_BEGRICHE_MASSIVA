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
#include<pthread.h>
#define SA struct sockaddr 
 
void function(int sock) 
{ 
   int fp;
   char *file_name = "info.txt";	
   ssize_t read_return;
   char buffer[BUFSIZ];
   recv(sock,buffer,sizeof(buffer),0);
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
        if (write(sock, buffer, read_return) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

}



//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8000 );
	
	//Bind
	if( bind(socket_desc,(SA*)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	
	while( (client_sock = accept(socket_desc, (SA*)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	

     return 0;
}


void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	
	//Receive a message from client
        function(sock);
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}
