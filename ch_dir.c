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