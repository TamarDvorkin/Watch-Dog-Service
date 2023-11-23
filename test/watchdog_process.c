

#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h> /*i/o printf*/
#include <stdatomic.h> /*atomic int*/
#include <unistd.h>     /* getpid */
#include <sys/types.h> /*pid_t*/
#include <signal.h>	   /*sigaction*/
#include <semaphore.h>  /* semaphor */
#include <stdlib.h> /*putenv, getenv*/
#include <pthread.h> 
#include <fcntl.h>/*O_CREAT*/
#include <sys/stat.h>
#include <unistd.h>/*GETPID*/
#include <errno.h>
#include <assert.h>/*ASSERT*/
#include <string.h>/*STRLEN*/

#include "../include/MMIIMP.h"
#include "../include/utilis.h" /*macros*/
#include "../include/watchdog.h"


#define IsDigit *(str+i)<58 && *(str+i)>47
#define IsCharBetweenAToZ *(str+i)>='A' && *(str+i)<='Z'
#define NUM_LEN_PID 6 /*num of digits in process num*/



void WD_PROCESS(char *argv[]);
static void InitArgStruct(thread_args_ty * args, char**argv);
int StringToInt(char *str, int base);

int main(int argc,  char *argv[])
{
	
     WD_PROCESS(argv);

     return 0;
}

void WD_PROCESS(char *argv[])
{
    sigset_t mask;
    sem_t *wd_sem = NULL;
    char pid[NUM_LEN_PID];
    thread_args_ty args;

    BuildMaskSign(&mask);
    pthread_sigmask(SIG_BLOCK, &mask,NULL);

    sprintf(pid, "%d", getpid());
	setenv("WD_PID", pid, 1); /*WD_PID*/
	
	printf("IM IN WD_PROCESSSSSS PID IS %d\n", getpid());
	
	
    wd_sem = sem_open("sem_wd_t", O_CREAT, 0644,0);
    
    InitArgStruct(&args, argv);
    printf("n is %ld\n", args.n);
    printf("n is %s\n", args.wd_path);
    printf("n is %s\n", args.user_path);
    printf("n is %u\n", args.interval);
    printf("n is %u\n", args.pid_to_watch);
    sem_post(wd_sem);
    
    MMI_imp(&args);
    
}

static void InitArgStruct(thread_args_ty * args, char**argv)
{
    int i = 0;

	args->n =StringToInt(argv[i],10);
	++i;

	args->interval = StringToInt(argv[i],10);
	++i;

	args->wd_path = argv[i];
	++i;

	args->user_path = argv[i];
	++i;

	args->pid_to_watch = StringToInt(argv[i],10);
	++i;
	
	while (NULL != argv[i])
	{
		args->argv[i] = argv[i];
		++i;
	}
	args->argv[i] = NULL;
}


int StringToInt(char *str, int base)
{
    int num=0,i=0,temp, size=0;

    size=strlen(str);
    if(base>36 || base<0)
	{
		printf("wrong base\n");
	}
	
	for(;i<size;++i)
	{
		if(IsDigit)/*check if digit between 0-9*/
		{
			if(base<=*(str+i)-'0')
			{ 
				return num ;
			}
			temp=*(str+i)-'0';
		}
			
		else if(IsCharBetweenAToZ)/*check if valid char*/
		{
			if(base<=*(str+i)-55)
			{
				return num;
			}
			temp=*(str+i)-55;
		}
			
		else 
		{
			return num;
		}
		num=num*base+temp;
	}

	return num;
}










