#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define lp(i,n) for(int i=0;i<n;i++)

void tostring(char str[], int num)
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
int exec_pinfo(char ** cmd)
{
	char pro[1000],pro2[1000];
	strcpy(pro,"/proc/");
	if(cmd[1]==NULL)
	{
		char str[10];
		tostring(str,getpid());
		strcat(pro,str);
	}
	else
		strcat(pro,cmd[1]);
	strcpy(pro2,pro);
	strcat(pro,"/status");
	FILE * file = fopen(pro,"r");
	int pid;
	char *pro_name=NULL;
	char status;
	ssize_t read;
	size_t l=1000;
	int f=0;
	while(read=getline(&pro_name,&l,file)!=-1)
	{
		if(strncmp(pro_name,"Name",4)==0 || strncmp(pro_name,"Pid",3)==0)
			printf("%s",pro_name);
		if(strncmp(pro_name,"State",5)==0)
		{
			int i=7;
			printf("Process Status: ");
			while(pro_name[i]!='\0')
				printf("%c",pro_name[i++]);
		}
		if(strncmp(pro_name,"VmSize",6)==0)
		{
			int i=11;
			printf("Virtual Memory: ");
			while(pro_name[i]!='\0')
				printf("%c",pro_name[i++]);
		}
	}
	strcat(pro2,"/exe");
	pro_name=malloc(l);
	readlink(pro2,pro_name,l);
	printf("Executable Path:%s\n",pro_name);
	free(pro_name);
	return 1;
}