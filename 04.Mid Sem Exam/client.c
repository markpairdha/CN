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

#define PORT1 55666
#define PORT2 55667
#define PORT3 55668
#define PORT4 55669

void merge(int a[], int m, int b[], int n, int sorted[]) {
  int i, j, k;
  j = k = 0;
  for (i = 0; i < m + n;) {
    if (j < m && k < n) {
      if (a[j] < b[k]) {
        sorted[i] = a[j];
        j++;
      }
      else {
        sorted[i] = b[k];
        k++;
      }
      i++;
    }
    else if (j == m) {
      for (; i < m + n;) {
        sorted[i] = b[k];
        k++;
        i++;
      }
    }
    else {
      for (; i < m + n;) {
        sorted[i] = a[j];
        j++;
        i++;
      }
    }
  }
}

int main(int argc, char* argv[])
{
	int sock1,sock2,sock3,sock4;
	struct sockaddr_in server1,server2,server3,server4;
	int list[1000],list1[250],list2[250],list3[250],list4[250],list_1[250],list_2[250],list_3[250],list_4[250];
	 int i,temp;
	 int sort1[500],sort2[750],sort[1000];

    for (i = 0; i < 1000; i++) {
      list[i]=(rand()%(1000 + 1));
    }

    for (i = 0; i < 250; i++) {
        list1[i]=list[i];
        list2[i]=list[i+250];
        list3[i]=list[i+500];
        list4[i]=list[i+750];
    }

	if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("[-] Error in creating sock1\n");
	}printf("[+] sock1 created\n");
  server1.sin_family = AF_INET;
  server1.sin_port = htons(PORT1);
 	server1.sin_addr.s_addr = inet_addr("127.0.0.1");
	if((connect(sock1,(struct sockaddr*)&server1, sizeof(server1))) < 0) {
		printf("[-]connection error of sock1\n");
		exit(1);
	}printf("[+]sock1 connected successfully\n");
	if (send(sock1, &list1,sizeof(list1),0) < 0) {
		printf("[-]Send failed of sock1\n");
		exit(1);
	}
	if (recv(sock1, &list_1,sizeof(list_1),0) < 0) {
		printf("[-]sock1 receiving failed\n");
		exit(1);
	} printf("[+] Received sock1 \n");
	close(sock1);


	if ((sock2 = socket(AF_INET, SOCK_STREAM,0)) < 0 ) {
		printf("[-] Error in creating sock2\n");
	}printf("[+] sock2 created\n");
  server2.sin_family = AF_INET;
  server2.sin_port = htons(PORT2);
 	server2.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ((connect(sock2,(struct sockaddr*)&server2, sizeof(server2))) < 0) {
		printf("[-]connection error of sock2");
		exit(1);
	}	printf("[+]sock2 connected successfully\n");
	if (send(sock2, &list2,sizeof(list2),0) < 0) {
		printf("[-]Send failed of sock2 \n");
		exit(1);
	}
	if (recv(sock2, &list_2,sizeof(list_2), 0) < 0) {
		printf("[-]sock2 receiving  failed\n");
		exit(1);
	} printf("[+] Received sock2 \n");
	close(sock2);


	if ((sock3 = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("[-] Error in creating sock1\n");
	}printf("[+] sock3 created\n");
	server3.sin_family = AF_INET;
	server3.sin_port = htons(PORT3);
	server3.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ((connect(sock3,(struct sockaddr*)&server3, sizeof(server3))) < 0) {
		printf("[-]connection error of sock3");
		exit(1);
	}printf("[+]sock3 connected successfully\n");
	if (send(sock3, &list3,sizeof(list3), 0) < 0) {
		printf("[-]Send failed of sock3\n");
		exit(1);
	}
	if (recv(sock3, &list_3,sizeof(list_3), 0) < 0) {
		printf("[-]sock1 receiving  failed of sock3\n");
		exit(1);
	} printf("[+] Received sock3 \n");
	close(sock3);


	if ((sock4 = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("[-] Error in creating sock4\n");
	}printf("[+] sock4 created\n");
	server4.sin_family = AF_INET;
	server4.sin_port = htons(PORT4);
	server4.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ((connect(sock4,(struct sockaddr*)&server4, sizeof(server4))) < 0) {
		printf("[-]connection error of sock4");
		exit(1);
	}printf("[+]sock4 connected successfully\n");
	if(send(sock4,&list4,sizeof(list4), 0) < 0) {
		printf("[-]Send failed of sock4\n");
		exit(1);
	}
	if (recv(sock4, &list_4,sizeof(list_4), 0) < 0) {
		printf("[-]sock4 receiving  failed");
		exit(1);
	} printf("[+] Received sock4 \n");
	close(sock4);

//merging
	merge(list_1, 250, list_2, 250, sort1);
	merge(sort1, 500, list_3, 250, sort2);
	merge(sort2, 750, list_4, 250, sort);
	for (i = 0; i < 1000; i++) {
		printf("%d\n", sort[i]);
	}
	return 0;
}
