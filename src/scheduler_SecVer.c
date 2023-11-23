
#include <stdlib.h>		/* for malloc & free */
#include <assert.h>	/* assert */
#include <stdio.h>	/* fprintf */
#include <unistd.h>	/* sleep */
#include <time.h>	/* time */
#include <unistd.h>  /*getpid*/

#include "../include/scheduler_SecVer.h"
#include "../include/Task.h"
#include "../include/pq_SecVer.h"


struct schedule
{
    pq_t *pq;/*  tasks_queue*/
    int Stop_Sched ; /*flag for run*/
    UID_t uid; /*last one who run before stop*/
    
};


/*	Description- 
	A function to determine the priorities of the variables in the queue.
	The function compares the priority of two elements.
	It returns 0 if the priority of task2 is lower than data and 1 if the reverse	*/

static int SortFunc(void *data, void *param);
priority_func_t sort_func = SortFunc;

/*	Description- 
	A function to determine if data matches param
	It returns 0 if data matches, else returns 1	 */

static int SamehUid (void *data, void *param);
pq_match_t same_uid = SamehUid;

/*	Description- 
	A function to destroy top task	 */

void SchedDestroyTopTsak(sch_ty *sched);      




sch_ty *SchedCreate(void)
{
	sch_ty *new_sched = (sch_ty*)malloc(sizeof(sch_ty));
	if(NULL == new_sched)
	{
		return NULL;
	}
	
	new_sched->pq = PqCreate(sort_func);
	if(NULL == new_sched->pq)
	{
		free(new_sched);
		return NULL;
	}
	
	new_sched->Stop_Sched = 0;
	new_sched->uid = BadUID;
	
	return new_sched;
}

void SchedDestroy(sch_ty *sched)
{
	
	assert(sched);
	SchedClear(sched);
	PqDestroy(sched->pq);
	sched->pq = NULL;
	free(sched);
	
}


/*Task create() and then Enqueue()*/
UID_t SchedAddTask(sch_ty *sched, void *param, op_func action, time_t interval)
{
	task_ty *new_task = NULL;
	
	assert(sched && action && interval);
	new_task = TaskCreate(param,action, interval);
	if(NULL == new_task)
	{
		
		return BadUID;
	}
	if (1 == PqEnqueue(sched->pq, (void *)new_task)) /*1 fail 0 success*/
	{
		TaskDestroy(new_task);
		return BadUID;
		
	}
	return new_task->uid;
	
}



void SchedStop(sch_ty *sched)
{
	assert(sched);
	sched -> Stop_Sched = 1; 
}



int SchedRemoveTask(sch_ty *sched, UID_t uid)
{
	task_ty *remove_task = NULL;
	assert(sched);
	
	if(SchedIsEmpty(sched))
	{
		return 1;
	}
	
	remove_task = PqErase(sched->pq, same_uid, &uid);
	
	if (NULL == remove_task)
	{
		return 1;
	}
	
	TaskDestroy(remove_task);
	return 0;
}


SCHED_RUN_RETURN_STATUS SchedRun(sch_ty *sched)
{

	task_ty *task_to_run = NULL;
	TASK_RUN_RETURN_STATUS status_task;
	time_t delta = 0;
	time_t next_run = 0;

	assert(sched);
	while(!(SchedIsEmpty(sched)) && (0 == sched->Stop_Sched )) /*  IsEmpty- 1 for empty, 0 for not empty*/ 
	{
		task_to_run = (task_ty *)PqDequeue(sched->pq);
		sched->uid = GetTaskUid(task_to_run);
		

		next_run = GetTaskNextTime(task_to_run);

		do
		{
			delta = next_run - time(NULL);
			sleep(delta); /*for tasks that take longer then the next time to run the next task*/
		}while(delta > 0);

		status_task = TaskRun(task_to_run);

		/*check every task status from user*/
		switch(status_task)
		{
			case DO_NOT_REPEAT:
			{
				TaskDestroy(task_to_run);
				break;
			}
			
			case REPEAT:
			{
				TaskUpdateTime(task_to_run);
				
				if (PqEnqueue(sched->pq, (void *)task_to_run)) /* if fail in enqueue -1 fail, 0 success*/
				{
					TaskDestroy(task_to_run);
					return ERROR_SCHED;
					
				}
				break;
			}
			
			case ERROR_TASK:
			{
				printf("Couldn't run the task. PID for the fail task is: %d \n", task_to_run->uid.pid); 
				TaskDestroy(task_to_run);
				return ERROR_SCHED;
			}
		}
		printf("stop flag is now  %d, pid now is %d \n", sched->Stop_Sched, getpid()); 
	}
	
	if (1 == sched->Stop_Sched)/* a task is in the  pq, and flag stop is on */ 
	{
		printf("\nstoped now\n");
		return STOPPED;
	}
	return SUCCESS;

}



UID_t SchedGetLastTask(sch_ty *sched)
{
	assert(sched);
	return sched->uid;
}


size_t SchedSize(const sch_ty *sched)
{

	assert(sched);
	return PqSize(sched->pq);

}

int SchedIsEmpty(const sch_ty *sched)
{
	assert(sched);
	return PqIsEmpty(sched->pq);
}

void SchedClear(sch_ty *sched)
{
	
	task_ty *task = NULL;
	assert(sched);
	task = PqDequeue(sched->pq);
	while(task)
	{
		TaskDestroy(task);
		task = PqDequeue(sched->pq);
	}

	
}



static int SortFunc (void *data, void *param)
{
	assert(data && param);
	
	return GetTaskNextTime((task_ty *)data) < GetTaskNextTime((task_ty *)param);	
}


static int SamehUid (void *data, void *param)
{
	assert(data && param);
	
	return UidIsSame(GetTaskUid((task_ty *)data), GetTaskUid((task_ty *)param)); 	/*UIDSame return 0 in success  1 in  unseccess */
}


void SchedDestroyTopTsak(sch_ty *sched)
{

	
	task_ty *task_to_remove = NULL;
	
	while(!SchedIsEmpty(sched))
	{
			
		task_to_remove = PqDequeue(sched->pq); 
		TaskDestroy(task_to_remove);	
	}
	
	
}
