
#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_


#include "heap.h"

typedef struct priority_queue pq_t; /*typedef struct priority_queue pq_t;*/

/*Description- 
A function to determine the priorities of the variables in the queue.
The function compares the priority of two elements.
It returns 0 if the priority of params is lower than that of data and one if the reverse*/

typedef int (*priority_func_t)(void *data, void *param);

/*Description- 
A function to determine if data matches param
It returns 1 if data matches, else returns 0 */

typedef int (*pq_match_t)(void *data, void *param);

/*******************************************************************************
*  Description:		create pq_t
*  Errors:		return NULL
*  Parameters:		compare function to manage sorting
*  Return value:	pointer to pq_t
*  Complexity:		O(1)
*******************************************************************************/
pq_t *PqCreate(priority_func_t cmp_func);

/*******************************************************************************
*  Description:		free all elements and the priority_queue_t from memory
*  Errors:		----
*  Parameters:		Pointer to pq_t
*  Return value:	----
*  Complexity:		O(n)
*******************************************************************************/
void PqDestroy(pq_t *queue);

/*******************************************************************************
*  Description:		add new element to the list according to the compare function
*  Errors:		----
*  Parameters:		pointer to pq_t and element to add to the queue
*  Return value:	0- success 1- fail
*  Complexity:		O(n)
*******************************************************************************/
int PqEnqueue(pq_t *queue, void *element);

/*******************************************************************************
*  Description:		remove front element
*  Errors:		----
*  Parameters:		pointer to pq_t, if empty rerturn null
*  Return value:	element to remove
*  Complexity:		O(1)
*******************************************************************************/
void *PqDequeue(pq_t *queue);

/*******************************************************************************
*  Description:		return the data of the front element
*  Errors:		----
*  Parameters:		pointer to pq_t, if empty rerturn null
*  Return value:	the front element
*  Complexity:		O(1)
*******************************************************************************/
void *PqPeek(const pq_t *queue);

/*******************************************************************************
*  Description:		check if the queue is empty
*  Errors:		----
*  Parameters:		pointer to pq_t
*  Return value:	0- success 1- fail
*  Complexity:		O(1)
*******************************************************************************/
int PqIsEmpty(const pq_t *queue);

/*******************************************************************************
*  Description:		count the number of the elements in the queue
*  Errors:		----
*  Parameters:		pointer to pq_t
*  Return value:	number of elements in the queue
*  Complexity:		O(n)
*******************************************************************************/
size_t PqSize(const pq_t *queue);

/*******************************************************************************
*  Description:		clear all elemets in queue
*  Errors:		----
*  Parameters:		pointer to pq_t
*  Return value:	----
*  Complexity:  	O(n)
*******************************************************************************/
void PqClear(pq_t *queue);

/*******************************************************************************
*  Description:		remove first encounter of element matches to param in match func
*  Errors:		----
*  Parameters:		pointer to pq_t, match function and param to compare
*  Return value:	element to remove,if empty\not found rerturn null
*  Complexity:		O(n)
*******************************************************************************/
void *PqErase(pq_t *queue, pq_match_t match_func, void *param);



#endif /*_PRIORITY_QUEUE_H_*/
