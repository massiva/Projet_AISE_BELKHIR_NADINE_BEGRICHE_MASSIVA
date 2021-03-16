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
	
	int sockfd, ret,c;
	
	int id =0;
	int newSocket;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	 sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8000);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(sockfd, (SA*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("Bind done \n");
	
	
	//Listen
	listen(sockfd , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);





	while(newSocket = accept(sockfd, (SA*)&serverAddr,  (socklen_t*)&c)){
	
		
		if(newSocket < 0){
			exit(1);
		}
		id++;
		printf("Connection accepted from %s:%d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
		memset(buffer, 0, MAX);
                recv(newSocket, buffer, MAX, 0);

 
                if (strcmp(buffer, "aurevoir") == 0)    //si le client ecrit aurevoir il est deconnecté du chat
                {
                    printf ("Connexion fermée pour le client %i\n",id);
                    shutdown(sockfd, SHUT_RDWR);
                    exit (0);
                }
 		else{
                printf ("client %d : %s\n",id,buffer);
                printf ("Réponse : ");
                fgets(buffer, MAX, stdin);
                buffer[strlen(buffer) - 1] = '\0';
                send(newSocket, buffer, strlen(buffer), 0);
		bzero(buffer,sizeof(buffer));
 		}
				}
			}
		}

	

	//close(newSocket);


	return 0;
}
