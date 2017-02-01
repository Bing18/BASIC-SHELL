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
void prompt(char *user,char host[],char imp[]){
	char cwd[400];
	if(getcwd(cwd,sizeof(cwd))<0)
		fprintf(stderr,"Unable to get working directory\n");
	else{
		int x=strlen(cwd);
		int y=strlen(imp),i;
		if(strcmp(imp,cwd)==0)
			printf("%s@%s:~$",user,host);
		else if(x>y && strstr(cwd,imp)!=NULL){
			char b[400];
			b[0]='~';
			for(i=y;i<x;i++)
				b[i-y+1]=cwd[i];
			b[x-y+1]='\0';
			printf("%s@%s:%s$",user,host,b);
		}
		else
			printf("%s@%s:%s$",user,host,cwd);
	}
}