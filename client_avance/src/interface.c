#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAX 1024

#define SA struct sockaddr

void func(int clientSocket) 
{
	char msg[MAX];
	
	int id=0;
        id+=1;
        printf ("Client : ");
        fgets(msg, MAX, stdin);// le client ecrit son message
        msg[strlen(msg) - 1] = '\0';
 
        if ((send(clientSocket, msg, sizeof(msg), 0)) == -1)
            perror("send");
	bzero(msg,sizeof(msg));
        recv(clientSocket, msg, MAX, 0);
        printf ("Serveur : %s\n", msg);	
	bzero(msg,sizeof(msg));

 }
int main(){

	int sock, ret;
	struct sockaddr_in server;

	
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

                 pthread_t sniffer_thread;
               while(1){
            
		
		pthread_create( &sniffer_thread , NULL , (void*)func , (void*)sock);
		//Now join the thread , so that we dont terminate before the thread
		pthread_join( sniffer_thread , NULL);}

	close(sock);

	return 0;
}
