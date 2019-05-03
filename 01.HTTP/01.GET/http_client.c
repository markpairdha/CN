#include<unistd.h>
#include <netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>
#include<string.h>
int main(int argc, char const *argv[])
{

	char message[1024];
	const char *host = argv[1];
	char *msg = "GET %s HTTP/1.0\r\n\r\n";
	struct hostent *server;
	server = gethostbyname(host);
	sprintf(message,msg,argv[2]);
	printf("%s\n",message);
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0)
	{
		printf("Socket creation error\n");
		exit(0);
	}
	int connect_status = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(connect_status < 0)
	{
		printf("Connection error\n");
		exit(0);
	}
	int i, byte;
	for(i = 0; i < strlen(message); i++)
	{
		byte = write(sockfd, &message[i], 1);
		if(byte == 0)
			break;
	}
	char ch;
	int bytes;
	while((bytes=read(sockfd,&ch,1)) == 1)
	{
		printf("%c",ch);
	}
	printf("%c\n",ch);
	return 0;
}
