/*MMI DNR*/
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 200112L /*setenv*/

#include <stdio.h>      /*fprintf*/
#include <time.h>       /**/
#include <stdlib.h>     /*getenv*/
#include <semaphore.h>  /* SemCreate,sem_getvalue,sem_view,sem_post */
#include <fcntl.h>      /* O_CREAT */
#include <string.h>     /*atoi*/
#include <signal.h>     /*sigusr2*/
#include <unistd.h>     /*getpid*/
#include <stdatomic.h>  /*atomic int*/
#include <sys/signal.h> /* SIG_UNBLOCK */
#include <assert.h>     /* assert */
#include <pthread.h>    /* pthread_create */
#include "../include/utilis.h" /* MACROS */
#include "../include/watchdog.h"
#include "../include/MMIIMP.h" /*args struct*/

#define WAIT 10

pid_t DNR_pid;



static void InitStruct(size_t n, unsigned int interval, char *wd_path, char **argv, thread_args_ty *args);

void DoNotResuscitata(void)
{
    char *pid_str = NULL;
    sigset_t mask;
    pid_t pid_to_terminate;
    char *dnr_pid_char = NULL;
    thread_args_ty args;
	
    dnr_pid_char=getenv("WD_PID");/*USER_PID WD_PID*/ 
    printf("><><dnr_pid_char is %s", dnr_pid_char);
    
    
    if(NULL!=dnr_pid_char)
    {
    		DNR_pid =atoi(dnr_pid_char);
    		
    		printf("getpid is %d DNR pid is %d\n", getpid(), DNR_pid);
    		
    		if (kill(DNR_pid, SIGUSR2) == -1)
    		{
        	printf("kill fail\n"); 
    		}
    		if (kill(getpid(), SIGUSR2) == -1)
    		{
       	 printf("kill fail\n"); 
    		}
    }
    
}

status_ty MakeMeImmortal(size_t n, unsigned int interval, char *wd_path, char **argv)
{

    thread_args_ty args;
    char wd_pid[PID_NUM_OF_DIGITS];
    sigset_t mask;
    pthread_t wd_thread; /*mmi thread- mmiImp thread func*/
    pthread_attr_t attr; /*SEG FAULT*/
    int result_thread_op = 0;
    /*struct timespec timeout;*/
    time_t time_to_stop;
    time_t cur_time;
    int status = -1;
    sem_t *sem_wd_t;

    assert(argv);
    assert(wd_path);

 

    sprintf(wd_pid, "%d", getpid());
    
    printf("getpid is <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<%d\n", getpid());

    InitStruct(n, interval, wd_path, argv, &args);

    
    sem_wd_t = sem_open("sem_wd_t", O_CREAT, 0644, 0); /*O_CREATE - create file for named semaphore */
    RETURN_IF_FAIL(sem_wd_t == SEM_FAILED, "FAIL TO OPEN SEMAPHORE", FAILURE)

    BuildMaskSign(&mask);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    
    pthread_create(&wd_thread, NULL, MMI_imp, (void *)&args);
    pthread_detach(wd_thread);
    

    time_to_stop = time(NULL) + WAIT;
    cur_time = 0;

    while (cur_time <= time_to_stop && 0 != status)
    {
        status = sem_trywait(sem_wd_t);
        cur_time = time(NULL);
    }
    sem_close(sem_wd_t);
    DNR_pid = args.pid_to_watch;
	
    return SUCCESSS;
}


static void InitStruct(size_t n, unsigned int interval, char *wd_path, char **argv, thread_args_ty *args)
{
	int i = 0;
	int count = 0;

    /*initialize all- args is the struct*/
    
    args->n = n;
    args->interval = interval;

    args->wd_path = wd_path;
    args->user_path = getenv("_");
    args->pid_to_watch = getpid();
    
    
    while(NULL!= argv[count])
    {
    	 ++count;
    }
    
    args->argv = (char **)malloc(sizeof(char *)*count);
    
    for(i = 0; i<count; ++i)
    {
    		
    		args->argv[i] = argv[i];
    }

}
