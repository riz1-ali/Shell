#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#define lp(i,n) for(int i=0;i<n;i++)

char * path_return(char * cur_wd,char * root)
{
	if(strcmp(root,cur_wd)==0)
		return "~";
	int root_s=strlen(root);
	int cur_wd_s=strlen(cur_wd);
	if(root_s > cur_wd_s)
		return cur_wd;
	int flag=0;
	lp(i,strlen(root))
	{
		if(root[i]!=cur_wd[i])
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		char *temp = (char*)malloc(200);
		temp[0]='~';
		temp[1]='/';
		lp(i,cur_wd_s-root_s-1)
			temp[i+2]=cur_wd[root_s+i+1];
		return temp;
	}
	else
		return cur_wd;
}

void print_prompt(char * root)
{
	char *usr=(char *)malloc(200*sizeof(char));
	char host[200];
	char cur_wd[1000];
	getcwd(cur_wd,sizeof(cur_wd));
	cuserid(usr);
	gethostname(host,200);
	char * path = path_return(cur_wd,root);
	printf("<\x1b[31m%s\033[m@\x1b[36m%s\033[m:\x1b[32m%s\033[m>",usr,host,path);
}