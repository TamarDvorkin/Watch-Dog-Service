#include <stdlib.h>/*for malloc, free*/
#include <stdio.h>/*for printf only*/
#include <assert.h>/*for assert*/
#include <stddef.h>	/*size_t*/
#include <time.h> /*for time_t*/
#include "../include/Task.h"



task_ty *TaskCreate(void *param, op_func action, unsigned int interval)
{
    task_ty *instance_task = (task_ty*)malloc(sizeof(task_ty));
    assert(action && interval);
    if(NULL == instance_task)
    {
    	printf("malloc error");
    	return NULL;
    }
    
    instance_task->param = param;
    instance_task->action = action;
    instance_task->time_interval_origin = interval;
    instance_task->next_time_to_run = time(NULL) + interval;
    instance_task->uid =   UidCreate(); 
    
    return instance_task;

}


void TaskDestroy(task_ty *task)
{
	assert(task);
	free(task);
	task = NULL;
}


void TaskUpdateTime(task_ty *task)
{
	assert(task);
	task->next_time_to_run = task->time_interval_origin + time(NULL) ;
	
}


TASK_RUN_RETURN_STATUS TaskRun(task_ty *task)
{
	assert(task);
	
	return task->action(task->param); 
}




time_t GetTaskNextTime(task_ty *task)
{
	assert(task);
	return task->next_time_to_run;

}

UID_t GetTaskUid(task_ty *task)
{
	assert(task);
	return task->uid;
}






























