#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<unistd.h>
#include<signal.h>
#include "get_command.h"
#include "split_command.h"
#include "print_prompt.h"
#include "ls.h"
#include "cd.h"
#include "pwd.h"
#include "pipe.h"
#include "pinfo.h"
#include "remindme.h"
#include "clock.h"
int b,xid;
int cmpp(const void * a,const void * b)
{
	return ( *(int*)b - *(int*)a );
}
void inthand(int sig)
{
	b=1;
	kill(xid,SIGSTOP);
}
int exec_command(char ** cur_cmd,int *a,char * root)
{
	b=0;
	int x=0,re=0,pi=0,file_ds[2],f=*a;
	while(cur_cmd[x]!=NULL)
	{
		if(strcmp(cur_cmd[x],">")==0 || strcmp(cur_cmd[x],"<")==0 || strcmp(cur_cmd[x],"<<")==0)
			re=1;
		if(strcmp(cur_cmd[x],"|")==0)
			pi=1;
		x++;
	}
	int fd,fo,inf=0,outf=0,inp,outp;
	if(pi==1)
	{
		return pipe_it(cur_cmd);
	}
	if(re==1)
	{
		char in[200],out[200];
		int stack[200],c=0;
		for(int i=0;i<x;i++)
		{
			if(strcmp(cur_cmd[i],"<")==0)
			{
				stack[c++]=i;
				if(access(cur_cmd[i+1],F_OK)==-1)
					return 0;
				strcpy(in,cur_cmd[i+1]);
				stack[c++]=i+1;
				inf=1;
				inp=dup(0);
			}
			if(strcmp(cur_cmd[i],">")==0)
			{
				stack[c++]=i;
				strcpy(out,cur_cmd[i+1]);
				stack[c++]=i+1;
				outf=1;
				outp=dup(1);
			}
			if(strcmp(cur_cmd[i],">>")==0)
			{
				stack[c++]=i;
				strcpy(out,cur_cmd[i+1]);
				stack[c++]=i+1;
				outf=2;
			}
		}
		qsort(stack,c,sizeof(int),cmpp);
		for(int i=0;i<c;i++)
		{
			int k=stack[i];
			while(cur_cmd[k+1]!=NULL)
			{
				cur_cmd[k]=cur_cmd[k+1];
				k++;
			}
			cur_cmd[k]=NULL;
		}
		if(inf)
		{
			fd=open(in,O_RDONLY);
			if(fd>0)
				dup2(fd,0);
			else
			{
				write(2,"File dosen't exist\n",strlen("File dosen't exist\n"));
				return 0;
			}
		}
		if(outf>=1)
		{
			struct stat buf;
			if(stat(out,&buf)==0)
			{
				if(outf==1)	
					fo=open(out,O_WRONLY | O_TRUNC | O_CREAT);
				else
					fo=open(out,O_WRONLY | O_APPEND);
				if(fo>0)
					dup2(fo,1);
			}
			else
			{
				fo=creat(out,0744);
				if(fo<0)
				{
					if(fd>0)
					{
						dup(0);
						close(fd);
					}
					if(outf==1)
						write(2,"Error Creating File\n",strlen("Error Creating File\n"));
					else
						write(2,"Error Opening File\n",strlen("Error Opening File\n"));
					return 0;	
				}
				dup2(fo,1);
			}
		}
	}
	x=0;
	pid_t pid;
	int status=1,sta;
	if(cur_cmd[0]==NULL)
		status=0;
	else if(strcmp(cur_cmd[0],"cd")==0)
	{
		if(f==0)
			status=exec_cd(cur_cmd,root);
		else
			status=1;
	}
	else if(strcmp(cur_cmd[0],"pwd")==0)
	{
		if(!f)
			status=exec_pwd();
		else
		{
			status=exec_pwd();
			if(status)
				printf("pwd with pid %d exited normally\n",getpid());
		}
	}
	else if(strcmp(cur_cmd[0],"clear")==0)
	{	
		printf("\033[H\033[J");
		if(f)
			printf("clear with pid %d exited no\n",getpid());
		status=1;
	}
	else if(strcmp(cur_cmd[0],"echo")==0)
	{
		if(!f)
			status=exec_echo(cur_cmd);
		else
		{
			status=exec_echo(cur_cmd);
			if(status)
				printf("echo with pid %d exited normally\n",getpid());
		}
	}
	else if(strcmp(cur_cmd[0],"ls")==0)
	{
		if(f==0)
			status=exec_ls(cur_cmd,root);
		else
		{
			pid=fork();
			if(pid==0)
			{
				status=exec_ls(cur_cmd,root);
				if(status)
					printf("ls with pid %d exited normally\n",getpid());
			}
		}
	}
	else if(strcmp(cur_cmd[0],"pinfo")==0)
	{
		if(!f)
			status=exec_pinfo(cur_cmd);
		else
		{
			status=exec_pinfo(cur_cmd);
			if(status)
				printf("pinfo with pid %d exited normally\n",getpid());
		}
	}
	else if(strcmp(cur_cmd[0],"setenv")==0)
	{
		if(!f)
			status=exec_setenv(cur_cmd);
		else
		{
			status=exec_setenv(cur_cmd);
			if(status)
				printf("echo with pid %d exited normally\n",getpid());
		}
	}
	else if(strcmp(cur_cmd[0],"unsetenv")==0)
	{
		if(!f)
			status=exec_unsetenv(cur_cmd);
		else
		{
			status=exec_unsetenv(cur_cmd);
			if(status)
				printf("echo with pid %d exited normally\n",getpid());
		}
	}
	else if(strcmp(cur_cmd[0],"remindme")==0)
	{
		pid=fork();
		int ex=0,st=0;
		if(pid==0)
		{
			if(cur_cmd[1]==NULL || cur_cmd[2]==NULL)
				st=0;
			else
				st=1;
			int j=0;
			while(cur_cmd[1][j]!='\0')
			{
				if(!isdigit(cur_cmd[1][j]))
					st=0;
				j++;
			}
			if(st)
			{
				ex=exec_remindme(cur_cmd);
				print_prompt(root);
				exit(0);
			}
		}
		status=ex;
	}
	else if(strcmp(cur_cmd[0],"clock")==0)
		status=exec_clock(cur_cmd);
	else if((strcmp(cur_cmd[0],"vi")==0 || strcmp(cur_cmd[0],"vim")==0) && f==1)
		status=1;
	else
	{
		status=1;
		pid=fork();
		status=pid;
		xid=pid;
		signal(SIGTSTP,inthand);
		if(pid==0)
		{
			signal(SIGINT,SIG_DFL);
			if(execvp(cur_cmd[0],cur_cmd)==-1)
			{
				if(re==1)
				{
					if(inf)
					{
						dup(0);
						close(fd);
					}
					if(outf>=1)
					{
						dup(1);
						close(fo);
					}
				}
			}
		}
		else if(f==0 && b==0)
			waitpid(pid,&sta,WUNTRACED);
	}
	if(re==1)
	{
		if(inf)
		{
			dup2(inp,0);
			close(fd);
		}
		if(outf>=1)
		{
			dup2(outp,1);
			close(fo);
		}
	}
	if(b==1)
		*a=2;
	return status;
}