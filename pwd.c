#include<stdio.h>
#include<unistd.h>

int exec_pwd()
{
	char cur_wd[1000];
	if(getcwd(cur_wd,sizeof(cur_wd))!=NULL);
	{
		printf("%s\n",cur_wd);
		return 1;
	}
	return 0;
}