#include<stdio.h>
#include<stdlib.h>

int main()
{
	srand(time(NULL));
	int i,k,values;
	printf("enter values: ");
	scanf("%d",&values); //10000
	FILE *fp = fopen("text.txt","w"); //writes into Desktop/16cs01032/text.txt.
	for(i=0;i<values;i++)
	{
		k=rand()%10000+1;
		fprintf(fp,"%d\n",k);
	}
	return 0;
}
