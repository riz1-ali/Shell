#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include "get_command.h"
char* get_command(int* ex)
{
	int sz=1000;
	char *str = malloc(sizeof(char)*sz);
	if(!str)
	{
		write(2,"Allocation Error\n",strlen("Allocation Error\n"));
		exit(EXIT_FAILURE);
	}
	int p=0,flag;
	while(1)
	{
		flag=getchar();
		if(flag==-1)
		{
			printf("\n");
			exit(0);
		}
		if(flag=='\n' || flag==EOF)
		{
			str[p]='\0';
			return str;
		}
		else
			str[p]=flag;
		p++;
		if(p>=sz)
		{
			sz*=2;
			str=realloc(str,sz);
			if(!str)
			{
				write(2,"Allocation Error\n",strlen("Allocation Error\n"));
				exit(EXIT_FAILURE);
			}
		}
	}
}
