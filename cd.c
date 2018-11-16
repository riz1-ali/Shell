#include<unistd.h>
#include<string.h>
#define lp(i,n) for(int i=0;i<n;i++)

int exec_cd(char ** cmd,char * root)
{
	if(cmd[1]==NULL)
	{
		chdir(root);
		return 1;
	}
	if(strcmp(cmd[1],"~")==0)
	{
		chdir(root);
		return 1;
	}
	if(cmd[1][0]=='~')
	{
		char * temp=malloc(400);
		int l=strlen(root);
		int m=strlen(cmd[1]);
		lp(i,l)
			temp[i]=root[i];
		temp[l]='/';
		lp(i,m-2)
			temp[l+1+i]=cmd[1][i+2];
		return !chdir(temp);
	}
	return !chdir(cmd[1]);
}