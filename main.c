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
#include "header.h"
#include "header1.h"
#include "header2.h"
#include "header3.h"
#include "header4.h"
#include "header5.h"
#include "header6.h"
sigjmp_buf env;
typedef struct bp
{	char pname[200];
	pid_t pid,pgid;
	struct bp *next;
}bp;
bp *background_process;

////////////
char g[2000];
int tam;
///////////
//int pipeHandler(char * args[],int number);
//int redirection(char *present_dir,char *arr[],int status,int number,int anu)
int pipeHandler(char *arr[],int number,char *present_dir,int status);
int redirection(char *present_dir,char *arr[],int status,int number,int anu);
int lets_echo(char *arr[],int n);
int call_back(char *arr[],int n);
void p_info(char* arr[],int number,char present_dir[]);
void prompt(char *user,char host[],char imp[]);
void insert_process(char name[],pid_t pid,pid_t pgid)
{
	bp *new=(bp *)malloc(sizeof(bp));
	strcpy(new->pname,name);
	new->pid=pid;
	new->pgid=pgid;
	new->next=NULL;
	if(background_process==NULL)
		background_process=new;
	else
	{
		bp *temp=background_process;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new;
	}
}
void remove_process(pid_t pid)
{
	if(background_process!=NULL)
	{
		bp *temp=background_process;
		if(background_process->pid==pid)
		{
			background_process=background_process->next;
			free(temp);
		}
		else
		{
			bp *r;
			while(temp!=NULL&&temp->pid!=pid)
			{
				r=temp;
				temp=temp->next;
			}
			if(temp!=NULL)
			{
				r->next=temp->next;
				free(temp);
			}
			else
				;
		}
	}
}
void delete_zombies(void)
{
	pid_t kidpid;
	int status;

	//printf("Inside zombie deleter:  ");
	while ((kidpid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		printf("\nChild %d terminated\n", kidpid);
		remove_process(kidpid);
	}
	siglongjmp(env,1);
}

bp *getname(pid_t pid)
{
	bp *temp=background_process;
	while(temp!=NULL&&temp->pid!=pid)
		temp=temp->next;
	return temp;
}

pid_t get_pid(int n)
{
	n--;
	bp *temp=background_process;
	while(temp!=NULL&&n--)
		temp=temp->next;
	if(temp!=NULL)
		return temp->pid;
	else
		return -1;
}
void fu(int signo)
{
	kill(tam,9);
}
/*void sig_handler(int signo)
{
  if (signo == SIGINT)
  {
  	fprintf(stdout,"\n");
  	char *user_name,system_name[200];
  	char present_dir[1000];
  	user_name=getenv("LOGNAME");//returns NULL if not present NULL in the string
	gethostname(system_name,200);//returns -1 on error and 0 if correct
	getcwd(present_dir,1000);//returns NULL terminated in a if error gives NULL in a
	prompt(user_name,system_name,present_dir);

	//exit(signo);
  }
    //printf("received SIGINT\n");
}*/
 void fuc(int signo)
{
	fprintf(stdout,"\n");
  	char *user_name,system_name[200];
  	char present_dir[1000];
  	user_name=getenv("LOGNAME");//returns NULL if not present NULL in the string
	gethostname(system_name,200);//returns -1 on error and 0 if correct
	//getcwd(present_dir,1000);//returns NULL terminated in a if error gives NULL in a
	prompt(user_name,system_name,g);
	fflush(stdout);

} 


int main()
{
	int shell_terminal=STDERR_FILENO;
	
	pid_t child_pid;pid_t shell_pgid;
	char delim[]=" \t\r\n\v\f";
        int  BUFFER_SIZE=1000;
	void delete_zombies(void);
	pid_t rohi;
	char present_dir[1000];
	char total_prompt[1000];
	char *user_name,system_name[200];
	char input[BUFFER_SIZE];
	char *token;
	user_name=getenv("LOGNAME");//returns NULL if not present NULL in the string
	gethostname(system_name,200);//returns -1 on error and 0 if correct
	getcwd(present_dir,BUFFER_SIZE);//returns NULL terminated in a if error gives NULL in a
	strcpy(g,present_dir);
	if(present_dir==NULL)
		fprintf(stderr,"opening current directory\n");
	strcpy(total_prompt,"~");
	/*if (signal(SIGINT, sig_handler) == SIG_ERR)
					 {

					  printf("\ncan't catch SIGINT\n");
					}
	*/
	while(1)
	{
		int bs=0;
		char* arr[1000];
		char* divide[1000];
		char super[1000];
		int k;
		for(k=0;k<1000;k++)
		{
			arr[k]='\0';
			divide[k]='\0';
			super[k]='\0';
		}

		//COMMAND PROMPT
		prompt(user_name,system_name,present_dir);
		/*if(signal(SIGINT,sig_handler)==SIG_ERR)
			perror("Signal not caught!!");*/
		//GETTING INPUT
		signal(SIGINT,fuc);
		char *rbs;
		rbs=fgets(super,BUFFER_SIZE,stdin);
		if(rbs==NULL)
				{
					printf("\n");
					exit(0);
				}

		long long int length=strlen(super);
		if(super[length-1]=='\n')
			super[length-1]='\0';
		token=strtok(super,";");
		int e=0;
		while(token !=NULL)
		{
			divide[e]=token;
			token=strtok(NULL,";");
			e++;
		}
		int q;
		int status;
		
		//char *dupl[1000];
		//int mn;
		//for(mn=0;mn<1000;mn++)
		//	strcpy(dupl[mn],divide[mn]);
		
		//printf("rem:%d\n",rem);
		//if(rem==0)
		//{
			//printf("yes i");
		for(q=0;q<e;q++)
		{
			//printf("%s\n",divide[q]);
			strcpy(input,divide[q]);
			//printf(" %s\n",input);

			//EXITS WITH THE 'exit' or 'quit' command
			if(strcmp(input,"exit") ==0 || strcmp(input,"quit")==0 )
			{
				return 0;
			}

			//GETTING THE COMMAND AND SPLITTING IT
			int i=0;
			token=strtok(input,delim);

			while(token !=NULL)
			{
				arr[i]=token;
				token=strtok(NULL,delim);
				i++;
			}
			int number=i;
			int rem,rem1=pipeHandler(arr,number,present_dir,status);
			//int pipeHandler(char *arr[],int number,char *present_dir,int status)
			if(rem1==0)
			rem=redirection(present_dir,arr,status,number,0);
			//printf("rem:%d\n",rem1);
			if(rem==0 && rem1==0)
			{



			//printf("%s\n",arr[0]);	
			if(strcmp(arr[0],"cd")==0)
			{

				ch_dir(arr,number,present_dir,total_prompt);
				//printf("%s\n",total_prompt);

			}
			else if(strcmp(arr[0],"pwd")==0)
			{
				char present[1000];
				if(getcwd(present,1000)<0)
				{
					fprintf(stderr,"SoME InValiD SynTAx\n");
				}
				else
				{
					printf("%s\n",present);
				}
			}
			else if(strcmp("pinfo",arr[0])==0)
			{
				p_info(arr,number,present_dir);

			}

			else if(arr[number - 1][0]=='&')
			{
				//printf("2");
				arr[number-1]=NULL;
				number--;	
				rohi=call_back(arr,number);
				insert_process(arr[0],rohi,rohi);	 

				struct sigaction sa;
				sigfillset(&sa.sa_mask);
				sa.sa_handler=(void *)delete_zombies;
				sa.sa_flags=0;
				sigaction(SIGCHLD,&sa,NULL);
				sigsetjmp(env,1);

			}
			//printf("%d\n",strlen(arr));
			

			else if(strcmp(arr[0],"jobs")==0)
			{
				//jobs();
				int i=1;
				bp *temp=background_process;
				while(temp!=NULL)
				{
					fprintf(stdout,"[%d]%s[%d]\n",i++,temp->pname,temp->pid);
					temp=temp->next;
				}
			}
			else if(strcmp(arr[0],"echo")==0)
			{
				int z,dem;
				char substr[2000];
				char* substr1;
				for(z=1;z<number;z++)
				{

					//dem =strlen(arr[z]);
					if(arr[z][0]=='"')
					{
						dem =strlen(arr[z]);
						for(int t=1;t<dem-1;t++)
						{
							arr[z][t-1]=arr[z][t];
							//substr[t-1]=arr[z][t];
						}
						arr[z][dem-2]='\0';
					}

					else if(arr[z][0]=='$')
					{

						dem=strlen(arr[z]);
						for(int t=1;t<dem;t++)
						{
							arr[z][t-1]=arr[z][t];
							//substr[t-1]=arr[z][t];
						}
						arr[z][dem-1]='\0';
						//substr[dem-1]='\0';

						arr[z]=getenv(arr[z]);
					}
				}

				//substr[t-1]=arr[z][t];
				lets_echo(arr,number);
			}
			else if(strcmp(arr[0],"killallbg")==0)
			{
				bp *temp=background_process;
				/*while(temp!=NULL)
				{
					remove_process(temp->pid);
					//char* rahul[2];
					//rahul[0]="kill";
					//rahul[1]=(int)temp->pid;	
					//execvp(*rahul,rahul);
					int rem=killpg(getpgid(temp->pid),9);
					if(rem<0)
					{
						fprintf(stderr,"error killing the process");
					}
					temp=temp->next;
				}*/
					while(temp!=NULL)
						{
					if(killpg(getpgid(temp->pid),9)<0)
					{
						fprintf(stderr,"error killing the process");
					}	
					//remove_process(temp->pid);
						temp=temp->next;
					}


			}

			else if(strcmp("kjob",arr[0])==0)
			{
				int ban;
				if(number==3)
				{
					ban=get_pid(atoi(arr[1]));
					if(ban>=0)
					{
						if(killpg(ban,atoi(arr[2]))<0)
						{
							fprintf(stderr, "error sending the signal\n");
						}
					}
					else
					{
						fprintf(stderr, "invalid job number \n");
					}

				}
				else
				{
					fprintf(stderr, "provide 2 arguments\n");
				}
			}
			
			else if(strcmp("fg",arr[0])==0)
			{
				//printf("yes\n");

				if(number<2 || number >2 )
				{
					fprintf(stderr, "provide only 1 argument\n");
					//exit(0);	
				}
				if(number==2)
				{
				//printf("yes1\n");
				int rb=atoi(arr[1]);
				bp *temp=background_process;
				int rb1=1;
				while(temp!=NULL)
				{
					if(rb==rb1)
						break;
					temp=temp->next;
					rb1++;
				}
				//printf("pid:%d\n",temp->pid);
				int child_pid;
				if(temp!=NULL)
				{
					
					remove_process(temp->pid);
					tam=temp->pid;
					signal(SIGINT,fu);
					//killpg(temp->pgid,SIGCONT);
					waitpid(temp->pid,&status,0);
					

				}

				else 
					fprintf(stderr,"invalid input\n");
				//remove_process(temp->pid);			
				}			
				
		}
			
						



			else
			{
				lets_echo(arr,number);
			}
			
			
			for(i=0;i<number;i++)
				arr[i]=NULL;
	}	}
	//}
	}

	return 0;
}


