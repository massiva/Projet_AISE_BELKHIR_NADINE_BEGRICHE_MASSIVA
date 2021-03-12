#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX 1024
#define SA struct sockaddr

int main(){
	
	int sockfd, ret;
	char msg[MAX];
	int id =0;
	int newSocket;
	struct sockaddr_in server;
	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	/* sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}*/

//Create socket
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if (sockfd == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8000 );
	
	//Bind
	if( bind(sockfd,(SA*)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(sockfd , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	//c = sizeof(struct sockaddr_in);





	while(1){
		newSocket = accept(sockfd, (SA*)&server, &addr_size);
		
		if(newSocket < 0){
			exit(1);
		}
		id++;
		printf("Connection accepted from %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
		memset(msg, 0, 255);
                recv(newSocket, msg, 255, 0);
 
                if (strcmp(msg, "aurevoir") == 0)    //si le client ecrit aurevoir il est deconnecté du chat
                {
                    printf ("Connexion fermée pour le client %i\n",id);
                    shutdown(sockfd, SHUT_RDWR);
                    exit (0);
                }
 
                printf ("client %d : %s\n",id,msg);
                printf ("Réponse : ");
                fgets(msg, 255, stdin);
                msg[strlen(msg) - 1] = '\0';
                send(newSocket, msg, strlen(msg), 0);
 
				}
			}
		}

	

	//close(newSocket);


	return 0;
}
