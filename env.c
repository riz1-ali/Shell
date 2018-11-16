#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "env.h"
int exec_setenv(char ** cmd)
{
	if(cmd[1] == NULL || cmd[4]!=NULL)
		return 0;
	if(cmd[2]==NULL)
		cmd[2]=" ";
	if(setenv(cmd[1],cmd[2],1)==-1)
		return 0;
	return 1;
}
int exec_unsetenv(char ** cmd)
{
	if(cmd[1] == NULL || cmd[2]!=NULL)
		return 0;
	if(unsetenv(cmd[1])==-1)
		return 0;
	return 1;
}