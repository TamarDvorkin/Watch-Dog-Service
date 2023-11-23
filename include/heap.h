
#ifndef _HEAP_H_
#define _HEAP_H_

#include <stddef.h> /* size_t */


/* Description: This is the heap priority function - for MIN
	if positve -  param>data
	if equal - 	param==data
	if negative - param<data	*/
typedef int (*cmp_func_ty)(const void *data, const void *param);

/* Description: Match param to an existing data in the heap */
typedef int (*heap_is_match_ty)(const void *data, const void *param);

typedef struct heap heap_ty;

/*******************************************************************************
*  Description:	Creates an array of a predefined size.
*  Return value: 	Success - Pointer to the created array, Failure - NULL.
*  Time Complexity: O(1).
*  Arguments: 		cmp_func: To perform the heapify operation.
*******************************************************************************/
heap_ty *HeapCreate(cmp_func_ty cmp_func); 

/*******************************************************************************
*  Description:	Destroy's the given heap.
*  Notes: 		If heap is NULL, no operation is done.
*  Time Complexity:	O(1).       
*******************************************************************************/
void HeapDestroy(heap_ty *heap);

/*******************************************************************************
*  Description:	Insert's a new object into the heap.
*  Error:			Undefined behavior if heap array is full.
*  Arguments: 		heap: The heap to push the item in. 
   				data: The data to insert.
*  Return value:  	0 - Success 1 - Allocation Error.
*  Time Complexity: O(log(n)).
*******************************************************************************/
int HeapPush(heap_ty *heap, void *data);

/*******************************************************************************
*  Description:	Removes the object with the highest priority.
*  Errors: 		Undefined behavior if heap array is empty.
*  Time Complexity: O(log(n)).
*******************************************************************************/
void *HeapPop(heap_ty *heap);

/*******************************************************************************
*  Description: 	Retrieve's the data of the highest priority object.
*  Errors: 		Undefined behavior if heap array is empty.
*  Return value:  	Pointer to the element with the highest priority in the heap.
*  Complexity:   	O(1).
*******************************************************************************/
void *HeapPeek(const heap_ty *heap);

/*******************************************************************************
*  Description:	Count's the number of objects in the heap.
*  Return value:  	The number of items in the heap.
*  Time Complexity: O(1) 
*******************************************************************************/
size_t HeapSize(const heap_ty *heap);

/*******************************************************************************
*  Description:	Check's if the heap is empty.		 
*  Return value: 	1 - heap is empty, 0 - heap is not empty.
*  Time Complexity: O(1).
*******************************************************************************/
int HeapIsEmpty(const heap_ty *heap);

/*******************************************************************************
*  Description:	Remove's the object with the corresponding value.
*  Notes: 		Undefined behavior if value does not exists.
*  Arguments: 		heap: The heap to remove the item from.
   				param: The object's corresponding data.
   				is_match: Function to match param to an existing data.
*  Return value:  	The data of the removed element.
*  Time Complexity: O(n).
*******************************************************************************/
void *HeapRemove(heap_ty *heap, heap_is_match_ty is_match, const void *param);


void HeapPrint(const heap_ty *heap);

#endif /* _HEAP_H_ */


