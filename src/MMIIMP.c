
#define _POSIX_C_SOURCE 200809L
/*unsetenv getenv*/

#include <stdio.h>      /* i/o handling */
#include <unistd.h>     /* getpid */
#include <sys/types.h> /*pid_t*/
#include <signal.h>	   /*sigaction*/
#include <stdatomic.h>   /*atomic_fetch_add*/
#include <semaphore.h>  /* semaphor */
#include <stdlib.h> /*exit*/
#include <string.h> /*strerror*/
#include <sys/wait.h>
#include <errno.h> /*perrno*/
#include <pthread.h> /*pthread_exit*/
#include <string.h> /*strdup*/


#include "../include/scheduler_SecVer.h"
#include "../include/watchdog.h"
#include "../include/MMIIMP.h"
#include "../include/utilis.h"

#define size 50
#define NUM_ELEMENT_IN_STRUCT 6 /*basic args- in ENUM*/
#define FLAG_OFF
#define FLAG_ON


atomic_int counter_g;
sch_ty *g_sched_thread;
int g_flag_sig2 = 0;
sem_t *sem_wd;


void ping_sigusr1_handler(int signum);
void ping_sigusr2_handlerTermn(int signum);
char **StructToArray(thread_args_ty *args);
static void InitSigAct(struct sigaction *Sig1Action, struct sigaction *Sig2Action);
static int CreateRunSched(unsigned int interval, thread_args_ty *args);
static void Clean(void);
int Revive(thread_args_ty *args_struct);
static TASK_RUN_RETURN_STATUS SendSignal(void *param);
static TASK_RUN_RETURN_STATUS CheckCounter(void *param);



char **struct_to_array;
int ID = 0;

/*********************************************Thread func************************************************************/
void *MMI_imp(void *a_struct) 
{
	
	char *pid_temp = NULL;
	char *pid_temp_char = NULL;
	pid_t pid_wd; /*to watch*/
	pid_t pid_user; /*current*/
	sem_t *imp_sem = NULL;
	char buff[200]= {0};
	
	struct timespec timeout = {0};
	int res;

	sigset_t mask;
	struct sigaction PingPong;/*Sig1Action*/
	struct sigaction Terminate;/*Sig2Action*/

	int status = 0;
	char **args = NULL;
	thread_args_ty *args_struct = (thread_args_ty*)a_struct;
	ID = getpid();
  
	/*decide which one is my target to send signals*/

	printf("IM IN MMIIMP THREAD\n");
	BuildMaskSign(&mask);
	InitSigAct(&PingPong, &Terminate);
	UnMaskSign(&mask);
	
	
	args = StructToArray(args_struct);
	
	if(NULL == getenv("WD_PID")) /*pid to watch*/ 
	{
		pid_wd = fork();
		if(-1 == pid_wd)
		{
			perror("fork fail in MMIIMP\n");
		}
		
		if(0 == pid_wd)
		{
			if(-1 == execv(((thread_args_ty*)args_struct)->wd_path , args))
			{
				perror("execv fail in MMIIMP\n");
			}
			
		}
	}
	
	else
	{
	
		pid_temp_char = getenv("WD_PID");
		pid_wd = atoi(pid_temp_char);
	}

	sprintf(buff, "%d", pid_wd);
	
	setenv("WD_PID", buff, 1);

	
	if(getpid() ==  pid_wd)
	{
		args_struct->pid_to_watch = pid_user;
	}
	else
	{
		args_struct->pid_to_watch = pid_wd;
	}
	

    CreateRunSched(args_struct->interval, args_struct);
    Clean(); 
	
	pthread_exit(NULL);

	return NULL;
}


/********************************************Sigaction, mask**********************************************************/

static void InitSigAct(struct sigaction *Sig1Action, struct sigaction *Sig2Action)
{
	Sig1Action->sa_handler = ping_sigusr1_handler;
    sigemptyset(&Sig1Action->sa_mask); /*mask to 0*/
	Sig1Action->sa_flags = 0;
	sigaction(SIGUSR1, Sig1Action, NULL);

    Sig2Action->sa_handler = ping_sigusr2_handlerTermn;
    sigemptyset(&Sig2Action->sa_mask); /*mask to 0*/
	Sig2Action->sa_flags = 0;
	sigaction(SIGUSR2, Sig2Action, NULL);
}


int BuildMaskSign(sigset_t *mask)
{

	sigemptyset(mask);
    sigaddset(mask, SIGUSR1);/*blocking specific signals- only SIGuser1,SIGuser2*/
    sigaddset(mask, SIGUSR2);
    
    return 0;

}

int UnMaskSign(sigset_t *mask)
{
	pthread_sigmask(SIG_UNBLOCK, mask,NULL);
	
	return 0;
}

/********************************************Create run sched + clean**********************************************************/

static int CreateRunSched(unsigned int interval, thread_args_ty *args)
{

	g_sched_thread=SchedCreate();
	SchedAddTask(g_sched_thread,args,SendSignal, interval);
	SchedAddTask(g_sched_thread,args, CheckCounter, interval);
	SchedRun(g_sched_thread);
	
}

static void Clean(void)
{
	SchedDestroy(g_sched_thread);
	if(0!=sem_unlink("sem_wd_t"))
	{
		
		if (errno == EACCES)
		{
			printf("The caller does not have permission to unlink this semaphore\n");
		}
		else if(errno == ENAMETOOLONG)
		{
			printf("name was too long\n");
		}
		else
		{
			printf("unknown error when unlink emaphore fail\n");
		}
	}

	if(0!=unsetenv("WD_PID"))
	{
		perror("failed clean wd_pid env var");
	}
	if(unsetenv("USER_PID")!=0)
	{
		perror("failed clean user_pid env var");
	}
}

/********************************************Init struct , tasks and signal handler***********************************************/

char **StructToArray(thread_args_ty *args_struct)
{
	
	char **newarray = NULL; /*argv*/
	char **args = NULL;
	char **argv = args_struct->argv;
	size_t size_argv; 
	size_t struct_size= 0;
	int i = 0;
	char temp[200];

	size_argv = sizeof(args_struct->argv)/sizeof(char *); 
	struct_size = sizeof(char *) * (size_argv + 1 + NUM_ELEMENT_IN_STRUCT);
	args = (char **)malloc(struct_size);
	
	
	sprintf(temp, "%lu", args_struct->n);
	args[i] = strdup(temp);
	++i;
	
	sprintf(temp, "%d", args_struct->interval);
	args[i] = strdup(temp);
	++i;

	args[i] = args_struct->wd_path;
	++i;

	args[i] = args_struct->user_path;
	++i;

	sprintf(temp, "%d", args_struct->pid_to_watch);
	args[i] = strdup(temp);
	++i;
	

	while (i<size_argv+NUM_ELENENT_IN_STRUCT)
	{
		args[i] = argv[i];
		++i;
	}

	args[i] ='\0'; 

 	return args;
}


static TASK_RUN_RETURN_STATUS SendSignal(void *param)
{

	/*send signal siguser1 and update counter*/

	int kill_res;
	int err;
	kill(((thread_args_ty*)param)->pid_to_watch, SIGUSR1);
	atomic_fetch_add(&counter_g, 1);

	return REPEAT;
}


static TASK_RUN_RETURN_STATUS CheckCounter(void *param)
{
	if (counter_g >= ((thread_args_ty*)param)->n) 
	{
		printf("IM IN MMIIMP THREAD- in task checkcounter before calling revive\n");
		Revive((thread_args_ty*)param);
	}

	return REPEAT;
}


void ping_sigusr1_handler(int signum)
{
	if (signum == SIGUSR1)
	{
		/*counter_g = 0;*/ /*TODO SET ATOMIC*/
		atomic_store(&counter_g,0);
	}
}

void ping_sigusr2_handlerTermn(int signum)
{

	(void)signum;
	printf("SIG2 getpid is %d\n", getpid());

	SchedStop(g_sched_thread);
}

/********************************************Revive**********************************************************/
int Revive(thread_args_ty *args_struct)
{

	pid_t new_pid;
	pid_t pid_wd;
	char **args = StructToArray(args_struct);
	char *pid_temp = NULL;

	new_pid = args_struct->pid_to_watch;
	if(0!=kill(new_pid, SIGKILL) )/*OR SEGTERM*/
	{
		perror("Error terminate process");
		return 1;
	}

	pid_temp = getenv("WD_PID");
	pid_wd = atoi(pid_temp);

	new_pid = fork();
	if(-1 == new_pid) 
	{
		perror("Error fork in revive func\n");
		return 1;
	}
	/*CHECK WHICH ONE FAIL : wd or client(for execv)? */

	if(0 == new_pid)
	{
		if(getpid()==pid_wd)
		{
			if(-1 == execv(args_struct->user_path, args))
			{
				perror("failed execvp client in Revive func\n");
			}

		}
		else
		{
			if(-1 == execv(args_struct->wd_path, args))
			{
				perror("failed execvp wd in Revive func\n");
			}
		}
	}

	return 0;
}



