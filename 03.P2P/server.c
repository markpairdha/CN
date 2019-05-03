#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

pthread_t tid[20];
int tind=0;

//Mark Sathish Pairdha
//16CS01032

struct user
{
	int port, socket;
	char ip[100];
}list [100];


int ind;

void *foo(void * n_s)
{
	char rec_buffer[100], send_buffer[100];
	struct user o = *(struct user *) n_s;
	int new_socket = o.socket;
	int i, j;

	while(1)
	{
		printf("\n[+]SERVER : Waiting for : %s %d\n", o.ip, o.port);
		memset( rec_buffer, 0, sizeof(rec_buffer) );
		recv( new_socket, &rec_buffer, sizeof(rec_buffer), 0 );
		printf("\n[+]SERVER : Proccessing request from : %s %d\n", o.ip, o.port);
		printf("%s\n", rec_buffer);

		if( strcmp( rec_buffer, "request" ) == 0 )
		{
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			printf("[+]SERVER : Size of the list : %d\n", ind);
			send( new_socket, &ind, sizeof(ind), 0 );
			printf("[+]The List : \n");
			for(i=0; i<ind; i++)
			{
				printf("%s %d\n", list[i].ip, list[i].port);
				if( strcmp( o.ip, list[i].ip) == 0 && o.port == list[i].port )
					continue;
				send( new_socket, &list[i].ip, sizeof(list[i].ip), 0 );
				send( new_socket, &list[i].port, sizeof(list[i].port), 0 );
			}
		}
		else if( strcmp(rec_buffer,"connect") == 0 )
		{
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			char rec_ip[100]; int rec_port;
			recv( new_socket, &rec_ip, sizeof(rec_ip), 0 );
			recv( new_socket, &rec_port, sizeof(rec_port), 0 );
			char this_ip[100];
			int this_socket, this_port;
			for(i=0; i<ind;i++)
			{
				if(strcmp( rec_ip, list[i].ip) == 0 && rec_port == list[i].port)
				{
					this_socket = list[i].socket;
					strcpy(this_ip,list[i].ip);
					this_port = list[i].port;
					for(j=i; j<ind; j++)
					{
						strcpy(list[j].ip,list[j+1].ip);
						list[j].port = list[j+1].port;
						list[j].socket = list[j+1].socket;
					}
					break;
				}
			}
			ind--;
			send( this_socket, &new_socket, sizeof(new_socket), 0 );
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			printf("[+]SERVER : Starting chat at the connect side\n");
			while(1)
			{
				recv( new_socket, &rec_buffer, sizeof(rec_buffer), 0 );
				if(strcmp(rec_buffer,"die")==0)
					break;
				send( this_socket, &rec_buffer, sizeof(rec_buffer), 0 );
				if(strcmp(rec_buffer,"end")==0)
					break;
			}
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			printf("[+]SERVER : chat was closed at the connect side\n");
		}
		else if( strcmp(rec_buffer,"wait") == 0 )
		{
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			strcpy(list[ind].ip, o.ip);
			list[ind].port = o.port;
			list[ind].socket = o.socket;
			ind++;
			char this_ip;
			int this_port, this_socket;
			printf("[+]SERVER : Waiting for a chat request\n");
			recv( new_socket, &this_socket, sizeof(this_socket), 0 );
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			printf("[+]SERVER : Got a chat request\n");
			while(1)
			{
				recv( new_socket, &rec_buffer, sizeof(rec_buffer), 0 );
				if(strcmp(rec_buffer,"die")==0)
					break;
				send( this_socket, &rec_buffer, sizeof(rec_buffer), 0 );
				if(strcmp(rec_buffer,"end")==0)
					break;
			}
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			printf("[+]SERVER : Chat was closed at the wait side\n");
		}
		else if(strcmp(rec_buffer,"logout") == 0)
		{
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			printf("[+]SERVER : Closing connection and removing from the list\n");
			close(new_socket);
			return 0;
		}
		else
		{
			printf("[-]SERVER : Something Wrong here\n");
			printf("\n[+]SERVER : AT : %s %d\n", o.ip, o.port);
			printf("[+]SERVER : Closing connection and removing from the list\n");
			close(new_socket);
			return 0;
		}
	}
}

int main()
{
	struct sockaddr_in client,server;
	int n, s;
	char rec_buffer[100], send_buffer[100];
	s = socket( AF_INET, SOCK_STREAM, 0 );
	server.sin_family = AF_INET;
	server.sin_port = 15528;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( bind( s, (struct sockaddr *)&server, sizeof(server) ) == -1 )
	{
		printf("[-]Binding failed..\n");
		return 1;
	}
	printf("\n[+]Server was started successfully.. waiting for response from client...\n");
	n = sizeof(client);
	int status = listen( s, 2 );
	struct user obj;
	while(1)
	{
		int new_socket = accept(s,(struct sockaddr *)&client,&n);
		printf("[+] SERVER : Got a new connection from -- %s %d\n\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		obj.socket = new_socket;
		obj.port = ntohs(client.sin_port);
		strcpy(obj.ip,inet_ntoa(client.sin_addr));
		pthread_create( &(tid[tind]), NULL, foo, (void *)&obj );
		tind++;
	}
	close(s);
	return 0;
}
