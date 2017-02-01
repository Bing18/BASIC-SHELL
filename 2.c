#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/prctl.h>
#include<errno.h>
#include<signal.h>
#include<setjmp.h>
sigjmp_buf env;
// TODO
//-> error messages to stderr not to stdout
//memory allocation sufficiency
//modular fashion

//some global declarations
//#define delim " "

char delim[]=" \t\r\n\v\f";
#define BUFFER_SIZE 1000

//char *arr[1000];//commands array
//char present_dir[1000];
//char total_prompt[1000];

//functions

int ch_dir(char* arr[],int n,char present_dir[],char total_prompt[])
{	
	//cd or cd ~
	if(n==1 || (arr[1][0]=='~' && strlen(arr[1])==1))
	{
		int rem=chdir(present_dir);
		if(rem<0)
		{
			//fprintf(STDERR,"Error while changing dir");
			fprintf(stderr,"Error while changing dir\n");
			return 0;
		}
		strcpy(total_prompt,present_dir);
	}
	else
	{

		//cd ~/file_name
		if(arr[1][0]=='~')
		{
			char present[1000];
			strcpy(present,present_dir);
			strcat(present,arr[1]+1);
			int rem=chdir(present);
			if(rem<0)
			{
				fprintf(stderr,"Error while changing dir\n");
				return 0;
			}
			//printf("%s\n",present);
			strcpy(total_prompt,present);

		}
		//cd .. or cd . or cd ./file_name
		else if(arr[1][0]=='.')
		{
			if(chdir(arr[1])<0)
			{
				fprintf(stderr,"Error while changing dir\n");
				return 0;
			}
			char present[1000];
			getcwd(present,1000);
			strcpy(total_prompt,present);
		}

		// cd 'file_name'
		else if(arr[1][0]!='/')
		{
			char present[1000];
			getcwd(present,1000);
			strcat(present,"/");
			strcat(present,arr[1]);
			int rem=chdir(present);
			if(rem<0)
			{
				fprintf(stderr,"Error while changing dir\n");
				return 0;
			}
			strcpy(total_prompt,present);
		}
		//cd /home/rahul 
		else
		{
			if(chdir(arr[1])<0)
			{
				//fprintf(2,"mvllvsdError while changing dir\n");
				fprintf(stderr,"Error while changing dir\n");
				return 0;
			}
			char present[1000];
			getcwd(present,1000);
			strcpy(total_prompt,present);
		}
	}
	return 0;
}

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

void delete_zombies(void)
{
	pid_t kidpid;
	int status;

	//printf("Inside zombie deleter:  ");
	while ((kidpid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		printf("\nChild %d terminated\n", kidpid);
	}
	siglongjmp(env,1);
}



int main()
{
	void delete_zombies(void);
	int rohi;
	char present_dir[1000];
	char total_prompt[1000];
	char *user_name,system_name[200];
	char input[BUFFER_SIZE];
	char *token;
	user_name=getenv("LOGNAME");//returns NULL if not present NULL in the string
	gethostname(system_name,200);//returns -1 on error and 0 if correct
	getcwd(present_dir,BUFFER_SIZE);//returns NULL terminated in a if error gives NULL in a
	if(present_dir==NULL)
		fprintf(stderr,"opening current directory\n");
	strcpy(total_prompt,"~");
	while(1)
	{
		/*int p,rohi;
		  p=waitpid(WAIT_ANY,NULL,WNOHANG);
		  if(rohi==p)
		  {
		  printf("process with pid %d exited normally\n",job[i].id);
		  continue;
		  }
		  signal(SIGCHLD,child);
		  return;*/

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


		char printed[1000];
		//int i;
		int a=strlen(present_dir);
		int b=strlen(total_prompt);
		if(a>b)
		{
			strcpy(printed,total_prompt);
		}
		else if(a==b)
		{
			strcpy(printed,"~");

		}
		else if(a<b)
		{
			int j;
			for(j=1;j<(b-a+1);j++)
			{
				printed[j]=total_prompt[a+j-1];
			}
			printed[0]='~';
			//printed[1]='/';
			printed[b-a+1]='\0';
		}
		//COMMAND PROMPT
		printf("%s@%s:%s$ ",user_name,system_name,printed);
		//GETTING INPUT
		fgets(super,BUFFER_SIZE,stdin);
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
		for(q=0;q<e;q++)
		{
			//printf("%s\n",divide[q]);
			strcpy(input,divide[q]);

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

			else if(arr[number - 1][0]=='&')
			{
				//printf("2");
				arr[number-1]=NULL;
				number--;	
				rohi=call_back(arr,number);
				struct sigaction sa;
				sigfillset(&sa.sa_mask);
				sa.sa_handler=(void *)delete_zombies;
				sa.sa_flags=0;
				sigaction(SIGCHLD,&sa,NULL);
				sigsetjmp(env,1);

			}
			//printf("%d\n",strlen(arr));


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

			else
			{
				lets_echo(arr,number);
			}
			for(i=0;i<number;i++)
				arr[i]=NULL;
		}





	}

	return 0;
}



