#include<stdio.h>
#include<string.h>
#include<signal.h>
static int run = 1;
void inthandler(int sig_num)
{
	run=0;
}
int exec_clock(char ** cmd)
{
	if(cmd[1]==NULL)
		return 0;
	int j=0;
	while(cmd[1][j]!='\0')
	{
		if(!isdigit(cmd[1][j]))
			return 0;
		j++;
	}
	run=1;
	int num=atoi(cmd[1]);
	if(num<=0)
		return 0;
	signal(SIGINT,inthandler);
	while(run)
	{
		FILE * time=fopen("/sys/class/rtc/rtc0/time","r");
		FILE * date=fopen("/sys/class/rtc/rtc0/date","r");
		char *pro_name=NULL;
		size_t l=100;
		getline(&pro_name,&l,date);
		pro_name[10]='\0';
		printf("%s ",pro_name);
		getline(&pro_name,&l,time);
		printf("%s",pro_name);
		sleep(num);
	}
	return 1;
}