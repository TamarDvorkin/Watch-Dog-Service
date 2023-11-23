#include<stdlib.h>/*for malloc, free*/
#include<assert.h>/*for assert*/
#include <stddef.h>	/*size_t*/
#include <stdio.h> /*printf only*/
#include "../include/pq_SecVer.h"
#include "../include/heap.h"


struct  priority_queue 
{
	
	heap_ty *heap;
};

pq_t *PqCreate(priority_func_t cmp_func)
{
	pq_t *priority_queue = NULL;
	assert(cmp_func);
	
	priority_queue = (pq_t *)malloc(sizeof(pq_t));
	if(NULL == priority_queue)
	{
		return NULL;
	
	}
	
    priority_queue->heap = HeapCreate((cmp_func_ty)cmp_func); /*(cmp_func_ty)cmp_func*/
    if (NULL == priority_queue->heap)
	{
		free(priority_queue);
    	return NULL;
    }
 	return priority_queue;

}



void PqDestroy(pq_t *queue)
{
	
	HeapDestroy(queue->heap);
	free(queue);

}



int PqEnqueue(pq_t *queue, void *element)
{
	int push_res; 
	assert(queue&& element); 
	push_res = HeapPush(queue->heap , element);

	
	return push_res;

}


void *PqDequeue(pq_t *queue)
{
	void *param;
	void *remove_res;
	
	assert(queue);
	
	if (HeapIsEmpty(queue->heap))/*1 empty*/
	{
		
		return NULL;
	}
	
	return HeapPop(queue->heap);
}


void *PqPeek(const pq_t *queue)
{
	assert(queue);
	if(HeapIsEmpty(queue->heap))
	{
		return NULL;
	}
	
	return HeapPeek(queue->heap);
}


int PqIsEmpty(const pq_t *queue)
{
	assert(queue);
	return HeapIsEmpty(queue->heap);
	
	
}

size_t PqSize(const pq_t *queue)
{
	assert(queue);
	return HeapSize(queue->heap);
}


void PqClear(pq_t *queue)
{
	assert(queue);
	while(!HeapIsEmpty(queue->heap)) /*while(!PqIsEmpty(queue))*/
	{
		PqDequeue(queue);
	}

}


void *PqErase(pq_t *queue, pq_match_t match_func, void *param)
{
	
	assert(queue); 
	assert(match_func); 
	assert(param); 
		
	return HeapRemove(queue->heap, (heap_is_match_ty)match_func, param); 	
}
	
	






