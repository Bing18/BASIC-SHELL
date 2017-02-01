#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
int lets_echo(char *arr[],int n)
{
	pid_t pid;
	int status;
	if((pid=fork())<0)
	{
		fprintf(stderr,"Forking child process failed\n");
		return 0;
	}
	else if(pid==0)
	{
		if(execvp(*arr,arr)< 0)
		{
			fprintf(stderr,"Enter A proper command\n");
			//fprintf("Enter A proper command");
			return 0;
		}
	}
	else
	{
		while(wait(&status)!=pid)
			continue;
	}
}