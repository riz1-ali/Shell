#include<stdio.h>
#include<stdlib.h>

int exec_echo(char ** cmd)
{
	if(cmd[1]==NULL)
		return 0;
	int i=1;
	while(cmd[i]!=NULL)
	{
		if(cmd[i][0]!='$')
			printf("%s ",cmd[i++]);
		else
		{
			char temp[100];
			int j=1;
			while(cmd[i][j]!=NULL)
			{
				temp[j-1]=cmd[i][j];
				j++;
			}
			if(getenv(temp))	
				printf("%s ",getenv(temp));
			else
				printf("%s ",cmd[i]);
			i++;
		}
	}
	printf("\n");
	return 1;
}