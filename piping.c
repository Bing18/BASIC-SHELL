#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>
#include "header5.h"
int redirection(char *present_dir,char *arr[],int status,int number,int anu);
int pipeHandler(char *arr[],int number,char *present_dir,int status)
{
	int i,flag,j,flag1,rem,cmds;
	pid_t pid;
	char *commands[1000];
	int fd1[2],fd2[2];
	flag=0;

	for(i=0;i<number;i++)
	{
		if(strcmp(arr[i],"|")==0)
		{
			flag=1;
			break;
		}

	}
	if(flag==0)
		return 0;
	else
	{
		cmds=0;
		i=0;
		rem=0;
		flag1=0;
		int l=0;
		while(arr[l]!=NULL)
		{
			if(strcmp(arr[l],"|")==0)
			{
				cmds++;
			}
			l++;
		}
		cmds++;//last command
		//storing the commands
		i=0;
		j=0;
		//em=0;
		//flag1=0;
		//printf("cmds:%d\n",cmds);
		while(arr[j]!=NULL && flag1 !=1)
		{
			//printf("yes\n");
			j=0;
			while(strcmp(arr[i],"|")!=0)
			{
				commands[j]=arr[i];
				i++;
				if(arr[i]==NULL)
				{
					flag1=1;
					j++;
					break;
				}
				j++;
			}
		commands[j]=NULL;
		i++;
		if(rem%2==0)
		{
			pipe(fd2);
		}
		else
		{
			pipe(fd1);
		}
		pid=fork();
		if(pid<0)
		{
			if(rem!=cmds-1)
			{
				if(rem%2==0)
					close(fd2[1]);
				else
					close(fd1[1]);
			}
			fprintf(stderr,"error while creating child process\n");	
			exit(0);
		}
		if(pid==0)
		{
			if(rem==0)
			{
				dup2(fd2[1],1);
			}
			else if(rem==cmds-1)
			{
				if(cmds%2==0)
				{
					dup2(fd2[0],0);

				}
				else
				{
					dup2(fd1[0],0);
				}
			}
			else
			{
				if(rem%2==0)
				{
					dup2(fd1[0],0);
					dup2(fd2[1],1);
				}
				else
				{
					dup2(fd1[1],1);
					dup2(fd2[0],0);
				}
			}
			int ccc=redirection(present_dir,commands,status,number,1);
		//	write(2,ccc,sizeof(int));
			//printf("no\n");
			if(ccc!=1)
			execvp(*commands,commands);
			//printf("mvsdp\n");

		}
		if(rem==0)
		{
			close(fd2[1]);
		}
		else if(rem==cmds-1)
		{
			if(cmds%2==0)
			{
				close(fd2[0]);
			}
			else
			{
				close(fd1[0]);
			}
		}
		else
		{
			if(rem%2==0)
				{
					close(fd1[0]);
					close(fd2[1]);
				}
				else
				{
					close(fd1[1]);
					close(fd2[0]);
				}

		}
		waitpid(pid,NULL,0);
		rem++;
	


		}




		}	
		return 1;
	}
