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
int redirection(char *present_dir,char *arr[],int status,int number,int anu)
{
	pid_t pid=0;
	int i;
	int input=0,output=0,dout=0;
	char in[1024];
	char out[1024];
	char dout1[1024];
	int presence=0;
	for(i=0;arr[i]!='\0';i++)
	{
		if(strcmp(arr[i],"<")==0)
		{
			strcpy(in,arr[i+1]);
			input=1;
			presence=1;
			arr[i]=NULL;
		}
		else if(strcmp(arr[i],">")==0)
		{
			strcpy(out,arr[i+1]);
			output=1;
			presence=1;
			arr[i]=NULL;
		}
		else if(strcmp(arr[i],">>")==0)
		{
			strcpy(dout1,arr[i+1]);
			dout=1;
			presence=1;
			arr[i]=NULL;
		}

	}
	if(presence==0)
		return 0;
	else
	{	if(anu==0)
		pid=fork();
		if(pid<0 && anu==0)
		{
			fprintf(stderr,"couldn't fork a child\n");
		}
		else if(pid==0)
		{
			if(input==1)
			{
				int desc1=open(in,0400);
				if(desc1<0)
				{
					fprintf(stderr,"error in opening input file\n");
					exit(0);
				}
				dup2(desc1,0);
				close(desc1);
			}
			if(output==1)
			{
				int desc2=creat(out,0644);
				if(desc2<0)
				{
					fprintf(stderr,"error in opening or creating output file\n");
					exit(0);
				}
				dup2(desc2,1);
				close(desc2);
			}
			if(dout==1)
			{
				int desc3=open(dout1,O_WRONLY | O_APPEND | O_CREAT,0777);
				chmod(dout1,0777);
				if(desc3<0)
				{
					fprintf(stderr,"error in opening or creating output file\n");
					exit(0);
				}
				dup2(desc3,1);
				close(desc3);
			}
			execvp(*arr,arr);
		}
		else if(pid>0 && anu==0)
		{
			while (!(wait(&status) == pid)) ;
		}
		for(i=0;i<number;i++)
				arr[i]=NULL;
		return 1;
	}
}