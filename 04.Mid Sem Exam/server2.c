#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>

#define PORT 44557

void quick_sort(int list[], int n)
{
	int c, d, t;
	for (c = 0; c < (n - 1); c++) {
		for (d = 0; d < n - c - 1; d++) {
			if (list[d] > list[d + 1]) {
				t = list[d];
				list[d] = list[d + 1];
				list[d + 1] = t;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	int sockfd, newsockfd, size, receive;
	int recv_buffer[250],ret,n;
	struct sockaddr_in server,client;


	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("[-] Error in creating socket\n");
	}  printf("[+] Socket created successsfully.\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");


		if ((ret= bind(sockfd, (struct sockaddr*)&server, sizeof(server))) < 0) {
		printf("[-]Binding error\n");
		exit(1);
	}printf("[+]Binding successful\n");

	listen(sockfd, 4);
	printf("[+]Listening .......");
	size = sizeof(struct sockaddr_in);
	if ((newsockfd = accept(sockfd, (struct sockaddr*)&client, (socklen_t*)&size)) < 0) {
		printf("[-]Binding  failed");
		exit(1);
	}printf("[+]Connection successfull\n");


	while ((receive = recv(newsockfd, &recv_buffer,sizeof(recv_buffer), 0)) > 0) {
		quick_sort(recv_buffer, 250);
		printf("Quick Sorted numbers : \n" );
		for(n=0;n<250;n++){
			printf("%d\t",recv_buffer[n]);
		}
		write(newsockfd, &recv_buffer,sizeof(recv_buffer));
	}
	if (receive == -1) {
		printf("[-]recv failed\n");
	}
	else if (receive == 0) {
		puts("[-]Client disconnected\n");
	}
	close(sockfd);
	close(newsockfd);
	return 0;
}
