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
#include "header2.h"
int call_back(char *arr[],int n)
{
	pid_t pid;
	int status;
	//p=waitpid(WAIT_ANY,NULL,WNOHANG);
	//int rem=getpid();
	//printf("%d\n",rem);
	if((pid=fork())<0)
	{
		fprintf(stderr,"Forking child process failed\n");
		return -1;
	}
	else if(pid==0)
	{
		//prevents creation of zombie process
		//prctl(PR_SET_PDEATHSIG,SIGHUP);
		//puts child process in new group
		setpgid(0,0);

		if(execvp(*arr,arr)< 0)
		{
			fprintf(stderr,"Enter A proper command\n");
			return pid;
		}
	}
	else
	{
		;
	}
	printf("[1] %d\n",pid);
	return pid;

}