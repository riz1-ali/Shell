#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
int cmpar(const void * a,const void * b)
{
	return ( *(int*)b - *(int*)a );
}
int pipe_it(char ** cur_cmd)
{
	pid_t pid;
	int num=0,c=0,x=0,i=0,a=0,file_ds[2],fds2[2],flag=1,re=0;
	while(cur_cmd[x]!=0)
	{
		if(strcmp(cur_cmd[x++],"|")==0)
			num++;
	}
	num++;
	char * cmd[200];
	while(flag)
	{
		c=0;
		re=0;
		while(strcmp(cur_cmd[i],"|")!=0 && cur_cmd[i]!=NULL)
		{
			if(strcmp(cur_cmd[i],">")==0 || strcmp(cur_cmd[i],"<")==0 || strcmp(cur_cmd[i],">>")==0)
				re=1;
			cmd[c++]=cur_cmd[i++];
			if(cur_cmd[i]==NULL)
			{
				flag=0;
				break;
			}
		}
		cmd[c]=NULL;
		i++;
		if(a%2)
			pipe(file_ds);
		else
			pipe(fds2);
		pid=fork();
		if(pid==0)
		{
			if(!a)
				dup2(fds2[1],1);
			else if(a==num-1)
			{
				if(num%2)
					dup2(file_ds[0],0);
				else
					dup2(fds2[0],0);
			}
			else
			{
				if(!a%2)
				{
					dup2(fds2[1],1);
					dup2(file_ds[0],0);
				}
				else
				{
					dup2(fds2[0],0); 
					dup2(file_ds[1],1); 
				}
			}
			if(re==1)
			{
				int fd,fo,inf=0,outf=0,inp,outp;
				char in[200],out[200];
				int stack[200],c=0;
				for(int i=0;cmd[i]!=NULL;i++)
				{
					if(strcmp(cmd[i],"<")==0)
					{
						stack[c++]=i;
						if(access(cmd[i+1],F_OK)==-1)
							return 0;
						strcpy(in,cmd[i+1]);
						stack[c++]=i+1;
						inf=1;
						inp=dup(0);
					}
					if(strcmp(cmd[i],">")==0)
					{
						stack[c++]=i;
						strcpy(out,cmd[i+1]);
						stack[c++]=i+1;
						outf=1;
						outp=dup(1);
					}
					if(strcmp(cmd[i],">>")==0)
					{
						stack[c++]=i;
						strcpy(out,cmd[i+1]);
						stack[c++]=i+1;
						outf=2;
					}
				}
				qsort(stack,c,sizeof(int),cmpar);
				for(int i=0;i<c;i++)
				{
					int k=stack[i];
					while(cmd[k+1]!=NULL)
					{
						cmd[k]=cmd[k+1];
						k++;
					}
					cmd[k]=NULL;
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
							fo=open(out,O_WRONLY | O_TRUNC);
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
				if(execvp(cmd[0],cmd)==-1)
				{
					kill(getpid(),SIGKILL);
					return 0;
				}
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
			else
			{
				if(execvp(cmd[0],cmd)==-1)
				{
					kill(getpid(),SIGKILL);
					return 0;
				}
			}
		}
		if(!a)
			close(fds2[1]);
		else if(a==num-1)
		{
			if(num%2)
				close(file_ds[0]);
			else
				close(fds2[0]);
		}
		else
		{
			if(!a%2)
			{
				close(fds2[1]);
				close(file_ds[0]);
			}
			else
			{
				close(file_ds[1]);
				close(fds2[0]);
			}
		}
		a++;
		waitpid(pid,NULL,0);
	}
	return 1;
}