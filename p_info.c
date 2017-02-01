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


void p_info(char* arr[],int number,char present_dir[])
{
				FILE *fp;
				char buf1[1256],buf2[1256];
				int pid;
				char name[1256],state;
				long unsigned int size;
				if(number==1)
				{
					sprintf(buf1,"/proc/%d/stat",getpid());                                 //open the stat and exe files
					sprintf(buf2,"/proc/%d/exe",getpid());
				}
				else
				{
					sprintf(buf1,"/proc/%s/stat",(arr[1]));
					sprintf(buf2,"/proc/%s/exe",(arr[1]));
				}
				if((fp=fopen(buf1,"r"))!=NULL)
				{
					/*scan the stat file*/
					fscanf(fp,"%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d",&pid,&state,&size);  
					fclose(fp);
					printf("pid -- %d\nProcess Status -- %c\nmemory -- %lu\n",pid,state,size);
					readlink(buf2,name,1256);
					if(strstr(name,present_dir))
					{
						char fi[1000]="~";
						strcat(fi,name+strlen(present_dir));
						printf("Executable Path -- %s\n",fi);                           //print print the stuff

					}
					else
						printf("Executable Path -- %s\n",name);
				}
				else
					fprintf(stderr,"No such process\n");
}