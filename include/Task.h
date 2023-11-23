#ifndef _TASK_H_
#define _TASK_H_

#include "uid.h"
#include <time.h> 	/*time*/


#ifndef _TASK_RUN_RETURN_STATUSENUM_
#define _TASK_RUN_RETURN_STATUSENUM_

typedef enum TASK_RUN_RETURN_STATUS
{
    DO_NOT_REPEAT = 0,
    REPEAT = 1,
    ERROR = 2
}TASK_RUN_RETURN_STATUS;


typedef TASK_RUN_RETURN_STATUS (*op_func)(void *param);     /* -1 stop task(not error) | 0 keep going | 1 error ---> according to ENUM TASK_RUN_RETURN_STATUS in sched.*/
#endif /*_TASK_RUN_RETURN_STATUSENUM_*/




struct task
{
    void *param;
    op_func action; 
    unsigned int time_interval_origin;   /* Interval between executions,unsigned int */
    time_t next_time_to_run;   /* next time the task should run*/
    
   
    UID_t uid;   /* UID of the task uid UID_t; */
};


typedef struct task task_ty;


/*******************************************************************************
*  Description:		Create a new task_ty object
*  Errors:			Return NULL
*  Parameters:		parameter, function, time interval
*  Return value:	Pointer to task_ty, or NULL
*  Complexity:		O(1)
*******************************************************************************/
task_ty *TaskCreate(void *param, op_func action, unsigned int interval);

/*******************************************************************************
*  Description:		Destroy a task
*  Errors:		    ---
*  Parameters:		Pointer to task_ty
*  Return value:	---
*  Complexity:		O(1)
*******************************************************************************/
void TaskDestroy(task_ty *task);

/*******************************************************************************
*  Description:		Update "next_time_to_run" in specific task 
					(add "time_interval_origin" to "next_time_to_run")
*  Errors:		    ---
*  Parameters:		Pointer to task_ty
*  Return value:	nun
*  Complexity:		O(1)
*******************************************************************************/
void TaskUpdateTime(task_ty *task);

/*******************************************************************************
*  Description:		Run task
*  Errors:		    ---
*  Parameters:		Pointer to task_ty
*  Return value:	the return value of the action function in the task_ty
*  Complexity:		O(1)
*******************************************************************************/
TASK_RUN_RETURN_STATUS TaskRun(task_ty *task);

/*******************************************************************************
*  Description:		return the time for next TaskRun.
*  Errors:		    ---
*  Parameters:		Pointer to task_ty
*  Return value:	time_t (unsinged int) object; 
*  Complexity:		O(1)
*******************************************************************************/
time_t GetTaskNextTime(task_ty *task);

/*******************************************************************************
*  Description:		return tasks uid
*  Errors:		    ---
*  Parameters:		Pointer to task_ty
*  Return value:	uid_ty object; 
*  Complexity:		O(1)
*******************************************************************************/
UID_t GetTaskUid(task_ty *task);


#endif /*_TASK_H_*/




