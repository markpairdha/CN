#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>


int main(){

	int clientSocket;
	pid_t pid;
	int newSocket, i = 0;
	char read_msg[600], write_msg[400];

	int port;
	printf("Enter port number : ");
	scanf("%d",&port);

	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	if(listen(clientSocket,5)==0)
		printf("Server started\n");
	else
		printf("Unable to start server\n");

	newSocket = accept(clientSocket, (struct sockaddr *) NULL,NULL);
	while(1){
		printf("Starting reading....\n");
		if(read(newSocket,read_msg,sizeof(read_msg)) < 0){
			perror("Reading error");
		}
		printf("Message read\n");
		//printf("%s\n",read_msg);
		char c = ' ';
		char* pos = strchr(read_msg,c);
		char file_name[20];
		memset(file_name , 0, strlen(file_name));
		//printf("file name : %s\n", file_name);
		int count = 0;
		char ch;
		while(pos[count+2] != ' '){
			file_name[count] = pos[count+2];
			count++;
		}

		FILE* file_page = NULL;
		printf("File name : %s\n",file_name);

		file_page = fopen(file_name, "r");

		if (file_page == NULL){
		  	printf("Cannot open file \n");
		  	exit(0);
		}

		char *res = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %s\n\n";
		char temp[10000];
		memset(temp , 0, strlen(temp));

		for(i = 0;i < strlen(res);i++)
			temp[i] = res[i];

		int ind = 0;
		ch = fgetc(file_page);

		while(ch != EOF){
			ind++;
			temp[i++] = ch;
			ch = fgetc(file_page);
		}

		char bcount[20];
		snprintf(bcount , 20 , "%d" , ind);

		char response[10000];
		memset(response , 0, strlen(response));
		printf("Copying to buffer\n");
		sprintf(response , temp , bcount);
		printf("Copy buffer complete\n");
		//printf("%s\n",response );
		int co = write(newSocket , response , strlen(response));
		//printf("%d\n", co);
		if( co < 0){
			printf("Server Unable to respond");
			exit(0);
		}
		printf("\nRespond complete.\n");
	}
	close(clientSocket);
	close(newSocket);

}
