#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<signal.h>
#include "exec_command.h"
#define lp(i,n) for(int i=0;i<n;i++)
char * root;
struct pro
{
	int pid;
	int status;
	char *ptr;
}arr_type;
int mark;
int ex=0;
int front=0;
struct pro arr[1000];
int run=1;
void tostr(char str[], int num)
{
    int rem,len=0,n;
    n=num;
    while(n!=0)
    {
        len++;
        n/=10;
    }
    lp(i,len)
    {
        rem=num%10;
        num=num/10;
        str[len-(i+1)]=rem+'0';
    }
    str[len]='\0';
}
int main(int argc, char const *argv[])
{
	root=getenv("PWD");
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	while(run)
	{
		int status,x,ex;
		ex=0;
		while((x=waitpid(-1,&status,WNOHANG))>0)
		{
			if(WIFEXITED(status))
			{
				lp(y,front)	
				{
					if(arr[y].pid==x)
					{
						arr[y].status=3;
						printf("%s with pid %d exited normally\n",arr[y].ptr,x);
						break;
					}
				}
			}
		}
		ex=0;
		char * cmd;
		char ** split_cmd;
		char ** cur_cmd;
		print_prompt(root);
		cmd=get_command();
		split_cmd=split_commands(cmd);
		int i=0,j,f;
		while(split_cmd[i]!=NULL)
		{
			if(strncmp(split_cmd[i],"setenv",6)==0 || strncmp(split_cmd[i],"unsetenv",8)==0)
			{
				int c=0,b=0;
				while(split_cmd[i][c]!=NULL)
				{
					if(split_cmd[i][c]=='[')
						b++;
					if(split_cmd[i][c]==']' && b!=0)
						b++;
					c++;
				}
				if(b%2==1)
					write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
			}
			cur_cmd=split_command2(split_cmd[i]);
			if(run==0)
			{
				run=1;
				continue;
			}
			int cs=0,re=0;
			j=0;
			f=0;
			int k=-1;
			if(front==0 && (strcmp(cur_cmd[0],"fg")==0 || strcmp(cur_cmd[0],"bg")==0))
			{
				if(cur_cmd[1]==NULL)
					write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
				i++;
				continue;
			}
			if(front==0 && strcmp(cur_cmd[0],"jobs")==0)
			{
				if(cur_cmd[1]!=NULL)
					write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
				i++;
				continue;
			}
			if(front==0 && strcmp(cur_cmd[0],"kjob")==0)
			{
				if(cur_cmd[1]==NULL)
					write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
				i++;
				continue;
			}
			while(cur_cmd[j]!=NULL)
			{
				if(strcmp(cur_cmd[j],"&")==0)
				{
					f=1;
					k=j;
					break;
				}
				j++;
			}
			if(k!=-1)
			{
				while(cur_cmd[k+1]!=NULL)
				{
					cur_cmd[k]=cur_cmd[k+1];
					k++;
				}
				cur_cmd[k]=NULL;
			}
			if(strcmp(cur_cmd[0],"jobs")==0)
			{
				if(cur_cmd[1]!=NULL)
				{
					write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
					i++;
					continue;
				}
				if(front==0)
					continue;
				int c=0;
				lp(y,front)
				{
					if(arr[y].status==2)
					{
						++c;
						char pro[1000];
						strcpy(pro,"/proc/");
						char temp[10];
						tostr(temp,arr[y].pid);
						strcat(pro,temp);
						strcat(pro,"/stat");
						FILE * file = fopen(pro,"r");
						char *pro_name=NULL;
						ssize_t read;
						size_t l=1000;
						int b=0,co=0,k=arr[y].pid,st=0;
						while(k>0)
						{
							st++;
							k/=10;
						}
						char status;
						while(read=getline(&pro_name,&l,file)!=-1)
						{
							for(int i=st;i<strlen(pro_name);i++)
							{
								if(pro_name[i]==' ' && b==0)
									co++;
								if(pro_name[i]=='(')
									b=1;
								if(pro_name[i]==')')
									b=0;
								if(co==2)
									f=1;
								if(f)
								{
									i++;
									f=0;
									status=pro_name[i];
									break;
								}
							}
						}
						if(status=='R')
							printf("[%d] Running %s[%d]\n",c,arr[y].ptr,arr[y].pid);
						else if(status=='Z')
							printf("[%d] Zombie %s[%d]\n",c,arr[y].ptr,arr[y].pid);
						else if(status=='I')
							printf("[%d] Idle %s[%d]\n",c,arr[y].ptr,arr[y].pid);
						else
							printf("[%d] Sleeping %s[%d]\n",c,arr[y].ptr,arr[y].pid);
					}
					else if(arr[y].status==3)
					{
						++c;
						printf("[%d] Stopped %s[%d]\n",c,arr[y].ptr,arr[y].pid);

					}
				}
				i++;
			}
			else if(strcmp(cur_cmd[0],"kjob")==0)
			{
				if(cur_cmd[1]==NULL || cur_cmd[2]==NULL)
				{
					write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
					i++;
					continue;
				}
				int c=0,f=0,id=atoi(cur_cmd[1]),sval=atoi(cur_cmd[2]);
				lp(y,front)
				{
					if(arr[y].status==2 || arr[y].status==3)
					{
						++c;
						if(c==id)
						{
							union sigval s;
							s.sival_int=1;
							if(sval==17 || sval==19 || sval==23)
								arr[y].status=3;
							if(sval==9)
								arr[y].status=4;
							sigqueue(arr[y].pid,sval,s);
							f=1;
							break;
						}
					}
				}
				i++;
				if(f==0)
					write(2,"Job dosen't exist\n",strlen("Job dosen't exist\n"));
			}
			else if(strcmp(cur_cmd[0],"fg")==0)
			{
				if(cur_cmd[1]==NULL)
				{
					write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
					i++;
					continue;
				}
				int id=atoi(cur_cmd[1]),c=0,f=0,pid;
				lp(y,front)
				{
					if(arr[y].status==2 || arr[y].status==3)
					{
						++c;
						if(c==id)
						{
							int status=0;
							f=1;
							kill(arr[y].pid,SIGSTOP);
							kill(arr[y].pid,SIGCONT);
							arr[y].status=4;
							waitpid(arr[y].pid,&status,WUNTRACED);
						}
					}
				}
				if(f==0)
					write(2,"Job dosen't exist\n",strlen("Job dosen't exist\n"));
				i++;
			}
			else if(strcmp(cur_cmd[0],"bg")==0)
			{
				if(cur_cmd[1]!=NULL)
				{
					int id=atoi(cur_cmd[1]),c=0,f=0;
					lp(y,front)
					{
						if(arr[y].status==2 || arr[y].status==3)
						{
							++c;
							if(c==id && arr[y].status==3)
							{
								f=1;
								arr[y].status=4;
								kill(arr[y].pid,SIGCONT);
							}
							else if(c==id && arr[y].status==2)
								f=1;
						}
					}
					if(f==0)
						write(2,"Invalid Job Conversion\n",strlen("Invalid Job Conversion\n"));
				}
				else
				{
					i++;
					continue;
				}
			}
			else if(strcmp(cur_cmd[0],"overkill")==0 && ex==0)
			{
				ex=1;
				lp(y,front)
				{
					if(arr[y].status!=4)
					{
						arr[y].status=4;
						kill(arr[y].pid,SIGKILL);
					}
				}
			}
			else
			{
				if(strcmp(cur_cmd[0],"exit")==0 || strcmp(cur_cmd[0],"quit")==0)
				{
					if(f==0)
						exit(0);
					else
						printf("%s with pid %d exited normally\n",cur_cmd[0],getpid());
					
				}
				else if(ex==0)
				{
					int state=exec_command(cur_cmd,&f,root);
					if(state==0)
						write(2,"Use proper commands and syntax\n",strlen("Use proper commands and syntax\n"));
					else if(state!=1 && f==1)
					{
						arr[front].pid=state;
						arr[front].status=2;
						arr[front].ptr=cur_cmd[0];
						front++;
					}
					else if(state!=1 && f==2)
					{
						arr[front].pid=state;
						arr[front].status=3;
						arr[front].ptr=cur_cmd[0];
						front++;
					}
				}
				i++;
			}
		}
	}
	return 0;
}
