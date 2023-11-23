
#ifndef _MMI_IMP_H
#define _MMI_IMP_H

#include <signal.h> /*sigset_t*/
#include <stddef.h>	/* size_t */


#define NUM_ELENENT_IN_STRUCT 6
#define PID_NUM_OF_DIGITS 11


typedef enum MMI_enum
{
	NUM_EXEC_PATH = 0,
	NUM_WD_PID =    1,
	NUM_N =         2,
	NUM_INTERVAL =  3,
	NUM_BASIC_ARGS
} MMI_enum_ty;



typedef struct thread_args
{
	
    size_t n;
	unsigned int interval;
	char *wd_path; /*target path to watch*/
	char *user_path;/*current path*/
	char **argv;
	pid_t pid_to_watch;

}thread_args_ty;

/*Thread function*/

void *MMI_imp(void *a_struct);

int BuildMaskSign(sigset_t *mask);
int UnMaskSign(sigset_t *mask);


#endif /*_MMI_IMP_H*/




