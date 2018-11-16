#include<stdio.h>
#include<string.h>
#include<unistd.h>

int exec_remindme(char ** cmd)
{
	sleep((int)atoi(cmd[1]));
	printf("\nReminder:");
	int i=2;
	while(cmd[i]!=NULL)
	{
		printf("%s ",cmd[i]);
		i++;
	}
	printf("\n");
	return 1;
}