
#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "uid.h"


typedef enum SCHED_RUN_RETURN_STATUS
{
    SUCCESS = 0, 
    STOPPED = 1,
    ERROR_SCHED = 2
}SCHED_RUN_RETURN_STATUS;


typedef struct schedule sch_ty;
typedef struct uid UID_ty;



#ifndef _TASK_RUN_RETURN_STATUSENUM_
#define _TASK_RUN_RETURN_STATUSENUM_

typedef enum TASK_RUN_RETURN_STATUS
{
    DO_NOT_REPEAT = 0,
    REPEAT = 1,
    ERROR_TASK = 2
}TASK_RUN_RETURN_STATUS;

/*	the action scheduler will run, 
	input: the function will receive void *param, 
	output: the output is acordin to the TASK_RUN_RETURN_STATUS enum */

typedef TASK_RUN_RETURN_STATUS (*op_func)(void *param);
#endif /*_TASK_RUN_RETURN_STATUSENUM_*/




 
/*******************************************************************************
*  Description:		Create a new scheduler
*  Errors:		    Return NULL
*  Parameters:		---
*  Return value:	Pointer to sch_t, or NULL
*  Complexity:		O(1)
*******************************************************************************/
sch_ty *SchedCreate(void);

/*******************************************************************************
*  Description:		Remove all tasks, and destroy the scheduler
*  Errors:		    ---
*  Parameters:		Pointer to sch_t
*  Return value:	---
*  Complexity:		O(n)
*******************************************************************************/
void SchedDestroy(sch_ty *sched);

/*******************************************************************************
*  Description:		Add a new task to the scheduler 
*  Errors:		    ---
*  Parameters:		Pointer to sch_t, time interval[SEC], FuncTask_t
*  Return value:	UID of the added task
*  Complexity:		O(n)
*******************************************************************************/
UID_t SchedAddTask(sch_ty *sched, void *param, op_func action, time_t interval);

/*******************************************************************************
*  Description:		Remove a specific task from the scheduler
*  Errors:		    ---
*  Parameters:		Pointer to sch_t, UID to a specific task
*  Return value:	0 - Success, 1 - Failure (if the task dose not exist)
*  Complexity:		O(n)
*******************************************************************************/
int SchedRemoveTask(sch_ty *sched, UID_t uid);

/*******************************************************************************
*  Description:		start the schedule 
*  Errors:		    will return STATUS enum with err status
*  Parameters:		Pointer to sch_t
*  Return value:	the return value of the last task to run (see STATUS enum)
*  Complexity:		O(n)
*******************************************************************************/
SCHED_RUN_RETURN_STATUS SchedRun(sch_ty *sched);  


/*******************************************************************************
*  Description:		return the uid of the last action performed
*  Errors:		    ---
*  Parameters:		Pointer to sch_t
*  Return value:	UID_ty
*  Complexity:		O(n)
*******************************************************************************/
UID_t SchedGetLastTask(sch_ty *sched);

/*******************************************************************************
*  Description:		Count the number of current tasks in the scheduler
*  Errors:		    ---
*  Parameters:		Pointer to sch_t
*  Return value:	Number of tasks
*  Complexity:		O(n)
*******************************************************************************/    
size_t SchedSize(const sch_ty *sched);

/*******************************************************************************
*  Description:		Check if the scheduler is empty
*  Errors:		    ---
*  Parameters:		Pointer to sch_t
*  Return value:	  0- success empty 1- fail
*  Complexity:		O(1)
*******************************************************************************/    
int SchedIsEmpty(const sch_ty *sched);

/*******************************************************************************
*  Description:		Clear all tasks from the scheduler
*  Errors:		    ---
*  Parameters:		Pointer to sch_t
*  Return value:	---
*  Complexity:		O(n)
*******************************************************************************/    
void SchedClear(sch_ty *sched);


void SchedStop(sch_ty *sched);




#endif /* __SCHEDULER_H__ */
