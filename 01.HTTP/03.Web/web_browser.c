
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
#include <netdb.h>


int main(int argc, char* argv[])
{
	int cnxn_status = 1,n = 0;
	char IP[20],type[4];
	struct sockaddr_in serverAddr;
	int clientSocket;
	char read_msg[3], write_msg[300] = "HEAD / HTTP/1.1\r\n\r\n";

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(80);

	FILE* fp = fopen("/home/mark/Desktop/fb_page.html","w");

	printf("Enter address type(ip/dn)\n");
	scanf("%s",type);

	if(strcmp(type,"dn") == 0){
		printf("Enter Server domain name\n");
		scanf("%s",IP);
		struct hostent* server_ip;
		server_ip = gethostbyname(IP);
		memcpy(&serverAddr.sin_addr, server_ip->h_addr_list[0], server_ip->h_length);
	}
	else if(strcmp(type , "ip")){
		printf("Enter Server IP address\n");
		scanf("%s",IP);
		serverAddr.sin_addr.s_addr = inet_addr(IP);
	}

	cnxn_status = connect(clientSocket, (struct sockaddr *) &serverAddr,sizeof(serverAddr));

	if(cnxn_status == 0){

		printf("Client Socket sucessfully connected\n");

		send(clientSocket,write_msg,sizeof(write_msg),0);
		printf("%s\n", write_msg);
		while(1){
			memset(read_msg,0,sizeof(read_msg));
			n = recv(clientSocket, read_msg,2, 0);
			printf("%s", read_msg);
			fprintf(fp,"%s",read_msg);

			if(n < 2)
				break;
		}
	}
	else
		printf("Socket connection failed\n");


	return 0;
}
