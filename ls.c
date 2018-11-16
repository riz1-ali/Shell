#include<unistd.h>
#include<sys/stat.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<stdio.h>
#include<dirent.h>
#define lp(i,n) for(int i=0;i<n;i++)

int exec_ls(char ** cmd,char * root)
{
	int l=0,a=0,i=1,minus=0,pos_l=0,pos_a=0;
	int lf[100],af[100];
	int lsz=0,asz=0;
	DIR * dir;
	struct dirent * ptr;
	struct stat file;
	struct passwd * pwd;
	struct group * grp;
	while(cmd[i]!=NULL)
	{
		int j=0;
		minus=0;
		if(cmd[i][0]=='~')
		{
			char * temp=malloc(400);
			int l=strlen(root);
			int m=strlen(cmd[1]);
			lp(a,l)
				temp[a]=root[a];
			temp[l]='/';
			lp(a,m-2)
				temp[l+1+a]=cmd[1][a+2];
			cmd[i]=temp;
		}
		while(cmd[i][j]!='\0')
		{
			if(cmd[i][j]=='-')
			{
				minus=1;
				j++;
				continue;
			}
			if(cmd[i][j]=='l' && minus==1)
			{
				pos_l=i;
				l=1;
				lf[lsz++]=i;
			}
			if(cmd[i][j]=='a' && minus==1)
			{
				pos_a=i;
				a=1;
				af[asz++]=i;
			}
			if(minus==1 && cmd[i][j]!='a' && cmd[i][j]!='l')
				return 0;
			j++;
		}
		i++;
	}
	i=1;
	int fl;
	while(cmd[i]!=NULL)
	{
		fl=0;
		lp(k,lsz)
		{
			if(i==lf[k])
			{
				fl=1;
				break;
			}
		}
		lp(k,asz)
		{
			if(i==af[k])
			{
				fl=1;
				break;
			}
		}
		if(fl)
		{
			i++;
			continue;
		}
		if(access(cmd[i],F_OK)==-1)
			return 0;
		i++;
	}
	if(l==0 && a==0)
	{
			int flag=0,f;
			i=1;
			while(cmd[i]!=NULL)
			{
				fl=0;
			lp(k,lsz)
			{
				if(i==lf[k])
				{
					fl=1;
					break;
				}
			}
			lp(k,asz)
			{
				if(i==af[k])
				{
					fl=1;
					break;
				}
			}
			if(fl)
			{
				i++;
				continue;
			}
			stat(cmd[i],&file);
			if(S_ISREG(file.st_mode))
			{
				printf("%s\n",cmd[i]);
				i++;
				flag=1;
				continue;
			}
			dir=opendir(cmd[i]);
			f=0;
			while((ptr=readdir(dir))!=NULL)
			{	
				if(ptr->d_name[0]!='.')
				{	
					if(f==0)
						printf("%s:\n",cmd[i]);
					printf("%s ",ptr->d_name);
					flag=1;
					f=1;
				}
			}
			if(f)
				printf("\n");
			i++;
		}
		if(flag==0 || strcmp(cmd[1],"~")==0)
		{
			char * cwd = malloc(1000);

			getcwd(cwd,1000);
			dir=opendir(cwd);
			while((ptr=readdir(dir))!=NULL)
				if(ptr->d_name[0]!='.')
					printf("%s ",ptr->d_name);
			printf("\n");
			free(cwd);
		}
	}
	if(l==0 && a==1)
	{
		int flag=0,f;
		i=1;
		while(cmd[i]!=NULL)
		{
			fl=0;
			lp(k,lsz)
			{
				if(i==lf[k])
				{
					fl=1;
					break;
				}
			}
			lp(k,asz)
			{
				if(i==af[k])
				{
					fl=1;
					break;
				}
			}
			if(fl)
			{
				i++;
				continue;
			}
			stat(cmd[i],&file);
			if(S_ISREG(file.st_mode))
			{
				printf("%s\n",cmd[i]);
				i++;
				flag=1;
				continue;
			}
			dir=opendir(cmd[i]);
			f=0;
			while((ptr=readdir(dir))!=NULL)
			{	
					if(f==0)
						printf("%s:\n",cmd[i]);
					printf("%s ",ptr->d_name);
					flag=1;
					f=1;
			}
			if(f)
				printf("\n");
			i++;
		}
		if(flag==0 || strcmp(cmd[1],"~")==0)
		{
			char * cwd = malloc(1000);
			getcwd(cwd,1000);
			dir=opendir(cwd);
			while((ptr=readdir(dir))!=NULL)
					printf("%s ",ptr->d_name);
			printf("\n");
			free(cwd);
		}
	}
	if(l==1 && a==0)
	{
		int flag=0,f;
		i=1;
		while(cmd[i]!=NULL)
		{
			fl=0;
			lp(k,lsz)
			{
				if(i==lf[k])
				{
					fl=1;
					break;
				}
			}
			lp(k,asz)
			{
				if(i==af[k])
				{
					fl=1;
					break;
				}
			}
			if(fl)
			{
				i++;
				continue;
			}
			stat(cmd[i],&file);
			if(S_ISREG(file.st_mode))
			{
				write(1,"-",strlen("-"));
				if((file.st_mode & S_IRUSR))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWUSR))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXUSR))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRGRP))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWGRP))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXGRP))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IROTH))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWOTH))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXOTH))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				printf(" %d ",file.st_nlink);
				pwd=getpwuid(file.st_uid);
				grp=getgrgid(file.st_gid);
				printf("%s %s ",pwd->pw_name,grp->gr_name);
				printf("%d ",file.st_size);
				char *time = ctime(&file.st_mtime);
				time[strlen(time)-1]='\0';
				printf("%s ",time);
				printf("%s\n",cmd[i]);
				i++;
				flag=1;
				continue;
			}
			dir=opendir(cmd[i]);
			f=0;
			while((ptr=readdir(dir))!=NULL)
			{	
				if(ptr->d_name[0]=='.')
					continue;
				stat(ptr->d_name,&file);
				if(S_ISDIR(file.st_mode))
					write(1,"d",strlen("d"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRUSR))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWUSR))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXUSR))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRGRP))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWGRP))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXGRP))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IROTH))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWOTH))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXOTH))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				printf(" %d ",file.st_nlink);
				pwd=getpwuid(file.st_uid);
				grp=getgrgid(file.st_gid);
				printf("%s %s ",pwd->pw_name,grp->gr_name);
				printf("%d ",file.st_size);
				char *time = ctime(&file.st_mtime);
				time[strlen(time)-1]='\0';
				printf("%s ",time);
				printf("%s\n",ptr->d_name);
				flag=1;
				f=1;
			}
			i++;
		}
		if(flag==0 || strcmp(cmd[1],"~")==0)
		{
			char * cwd = malloc(1000);
			getcwd(cwd,1000);
			dir=opendir(cwd);
			free(cwd);
			while((ptr=readdir(dir))!=NULL)
			{
				if(ptr->d_name[0]!='.')
				{
					stat(ptr->d_name,&file);
					if(S_ISDIR(file.st_mode))
						write(1,"d",strlen("d"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IRUSR))
						write(1,"r",strlen("r"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IWUSR))
						write(1,"w",strlen("w"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IXUSR))
						write(1,"x",strlen("x"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IRGRP))
						write(1,"r",strlen("r"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IWGRP))
						write(1,"w",strlen("w"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IXGRP))
						write(1,"x",strlen("x"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IROTH))
						write(1,"r",strlen("r"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IWOTH))
						write(1,"w",strlen("w"));
					else
						write(1,"-",strlen("-"));
					if((file.st_mode & S_IXOTH))
						write(1,"x",strlen("x"));
					else
						write(1,"-",strlen("-"));
					printf(" %d ",file.st_nlink);
					pwd=getpwuid(file.st_uid);
					grp=getgrgid(file.st_gid);
					printf("%s %s ",pwd->pw_name,grp->gr_name);
					printf("%d ",file.st_size);
					char *time = ctime(&file.st_mtime);
					time[strlen(time)-1]='\0';
					printf("%s ",time);
					printf("%s\n",ptr->d_name);
				}
			}	
		}
	}
	if(l==1 && a==1)
	{
		int flag=0,f;
		i=1;
		while(cmd[i]!=NULL)
		{
			fl=0;
			lp(k,lsz)
			{
				if(i==lf[k])
				{
					fl=1;
					break;
				}
			}
			lp(k,asz)
			{
				if(i==af[k])
				{
					fl=1;
					break;
				}
			}
			if(fl)
			{
				i++;
				continue;
			}
			stat(cmd[i],&file);
			if(S_ISREG(file.st_mode))
			{
				write(1,"-",strlen("-"));
				if((file.st_mode & S_IRUSR))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWUSR))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXUSR))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRGRP))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWGRP))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXGRP))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IROTH))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWOTH))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXOTH))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				printf(" %d ",file.st_nlink);
				pwd=getpwuid(file.st_uid);
				grp=getgrgid(file.st_gid);
				printf("%s %s ",pwd->pw_name,grp->gr_name);
				printf("%d ",file.st_size);
				char *time = ctime(&file.st_mtime);
				time[strlen(time)-1]='\0';
				printf("%s ",time);
				printf("%s\n",cmd[i]);
				i++;
				flag=1;
				continue;
			}
			dir=opendir(cmd[i]);
			f=0;
			while((ptr=readdir(dir))!=NULL)
			{	
				stat(ptr->d_name,&file);
				if(S_ISDIR(file.st_mode))
					write(1,"d",strlen("d"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRUSR))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWUSR))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXUSR))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRGRP))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWGRP))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXGRP))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IROTH))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWOTH))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXOTH))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				printf(" %d ",file.st_nlink);
				pwd=getpwuid(file.st_uid);
				grp=getgrgid(file.st_gid);
				printf("%s %s ",pwd->pw_name,grp->gr_name);
				printf("%d ",file.st_size);
				char *time = ctime(&file.st_mtime);
				time[strlen(time)-1]='\0';
				printf("%s ",time);
				printf("%s\n",ptr->d_name);
				flag=1;
				f=1;
			}
			i++;
		}
		if(flag==0 || strcmp(cmd[1],"~")==0)
		{
			char * cwd = malloc(1000);
			getcwd(cwd,1000);
			dir=opendir(cwd);
			free(cwd);
			while((ptr=readdir(dir))!=NULL)
			{
				stat(ptr->d_name,&file);
				if(S_ISDIR(file.st_mode))
					write(1,"d",strlen("d"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRUSR))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWUSR))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXUSR))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IRGRP))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWGRP))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXGRP))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IROTH))
					write(1,"r",strlen("r"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IWOTH))
					write(1,"w",strlen("w"));
				else
					write(1,"-",strlen("-"));
				if((file.st_mode & S_IXOTH))
					write(1,"x",strlen("x"));
				else
					write(1,"-",strlen("-"));
				printf(" %d ",file.st_nlink);
				pwd=getpwuid(file.st_uid);
				grp=getgrgid(file.st_gid);
				printf("%s %s ",pwd->pw_name,grp->gr_name);
				printf("%d ",file.st_size);
				char *time = ctime(&file.st_mtime);
				time[strlen(time)-1]='\0';
				printf("%s ",time);
				printf("%s\n",ptr->d_name);
			}	
		}
	}
	return 1;
}