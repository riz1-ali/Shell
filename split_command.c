#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "split_command.h"

char ** split_commands(char* cmd)
{
	int sz=1000,p=0;
	char ** splits=malloc(sizeof(char*)*sz);
	if(!splits)
	{
		write(2,"Allocation Error\n",strlen("Allocation Error\n"));
		exit(EXIT_FAILURE);
	}
	char* temp=strtok(cmd,";");
	while(1)
	{
		if(temp!=NULL)
		{
			splits[p++]=temp;
			if(p>=sz)
			{
				sz*=2;
				splits=realloc(splits,sizeof(char*)*sz);
				if(!splits)
				{
					write(2,"Allocation Error\n",strlen("Allocation Error\n"));
					exit(EXIT_FAILURE);
				}
			}
			temp=strtok(NULL,";");
		}
		else
			break;
	}
	splits[p]=NULL;
	return splits;
}

char ** split_command2(char* cmd)
{
	int sz=1000,p=0;
	char ** splits=malloc(sizeof(char*)*sz);
	if(!splits)
	{
		write(2,"Allocation Error\n",strlen("Allocation Error\n"));
		exit(EXIT_FAILURE);
	}
	char* temp=strtok(cmd," \n\t[]");
	while(1)
	{
		if(temp!=NULL)
		{
			splits[p++]=temp;
			if(p>=sz)
			{
				sz*=2;
				splits=realloc(splits,sizeof(char*)*sz);
				if(!splits)
				{
					write(2,"Allocation Error\n",strlen("Allocation Error\n"));
					exit(EXIT_FAILURE);
				}
			}
			temp=strtok(NULL," \n\t[]");
		}
		else
			break;
	}
	splits[p]=NULL;
	return splits;
}