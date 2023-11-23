
#include <stdlib.h>		/* malloc, free */
#include <assert.h>		/* assert */ 
#include <stdio.h>		/* fprintf, */
#include <stddef.h> 	/* size_t */
#include <string.h>		/* memcpy */
#include <sys/types.h>  /* ssize_t */

#include "../include/vector_third.h"
#include "../include/heap.h"


#define	CAPACETY 200 
#define PARENT_INDEX(i) (i-1)/2
#define L_CHILD_INDEX(i) (2*i)+1
#define R_CHILD_INDEX(i) (2*i)+2
#define ROOT_INDEX 0
#define LAST_INDEX(heap) HeapSize(heap) - 1
#define ZERO 0
#define ONE 1


struct heap
{
	cmp_func_ty cmp_func;
	vector *heap_arr;
};

static size_t HeapifyUp(heap_ty *heap, size_t ele_index);
static size_t HeapifyDown(heap_ty *heap , size_t idx);
static void Swap(void **x, void **y); 
static ssize_t HeapFind(heap_ty *heap ,heap_is_match_ty is_match ,void* param);
static size_t GetParent(size_t ele_index);


heap_ty *HeapCreate(cmp_func_ty cmp_func)
{
	
	heap_ty *heap = NULL; 
	
	assert(cmp_func); 
	
	heap = (heap_ty *)malloc(sizeof(heap_ty));
	
	if (NULL == heap)
    {
        fprintf(stderr,"Alocation Error");
        return NULL;
    }
	
	heap->heap_arr = DVectorCreate(sizeof(void *), CAPACETY);
	
	if (NULL == heap->heap_arr)
    {
        fprintf(stderr,"Alocation Error ");
        free(heap);
        heap = NULL;
        
        return NULL;
    }
    
    heap->cmp_func = cmp_func;
    
    return heap;
}

void HeapDestroy(heap_ty *heap)
{
	DVectorDestroy(heap->heap_arr);
	free(heap);
	heap = NULL;
}


int HeapPush(heap_ty *heap, void *data)
{
	size_t push_i = 0;
	int status = 0;  
	
	assert(heap); 
	assert(data); 
	
	push_i = HeapSize(heap);
	status = DVectorPushBack(heap->heap_arr, &data);
	if(ZERO != status)
	{
		return ONE; 
	}
	
	HeapifyUp(heap, push_i);
	
	return ZERO; 
}


void *HeapPop(heap_ty *heap)
{
	
	void *last_val   = NULL; 
	void *root_val   = NULL; 
	void *return_val = NULL;
	
	assert(heap); 
	
	if(ONE == HeapIsEmpty(heap))
	{
		return NULL;
	}
	
	if(ONE == HeapSize(heap))
	{
		
		root_val = HeapPeek(heap);
		DVectorPopBack(heap->heap_arr);
		
		return root_val;
	}

	last_val   = *(void **)DVectorGetElement( heap->heap_arr , LAST_INDEX(heap));
	root_val   = HeapPeek(heap);
	Swap(last_val , root_val); 
	DVectorPopBack(heap->heap_arr);
	HeapifyDown(heap, ROOT_INDEX);
	
	return last_val;
}


void *HeapPeek(const heap_ty *heap)
{
	assert(heap); 
	
	return *(void **)DVectorGetElement(heap->heap_arr, ROOT_INDEX);
}


size_t HeapSize(const heap_ty *heap)
{
	assert(heap); 
	
	return DVectorSize(heap->heap_arr);
}

int HeapIsEmpty(const heap_ty *heap)
{
	assert(heap); 
	
	return HeapSize(heap) == ZERO? ONE: ZERO;
}

void *HeapRemove(heap_ty *heap, heap_is_match_ty is_match, const void* param)
{
	size_t size = 0;
	size_t i = 0;
	void **data = NULL;
	
	assert(heap);
	assert(is_match);
	
	size = HeapSize(heap);
	
	for(i = 0; i < size; ++i)
	{
		data = *(void**)DVectorGetElement(heap->heap_arr, i);
		
		if(ZERO == is_match(data , param))
		{
			
			Swap(DVectorGetElement(heap->heap_arr, i), DVectorGetElement(heap->heap_arr, HeapSize(heap) - 1));
			data = *(void**)DVectorGetElement(heap->heap_arr, HeapSize(heap) - 1);
			DVectorPopBack(heap->heap_arr);
	
			if(ZERO == HeapIsEmpty(heap))
			{
				i = HeapifyUp(heap, i);
				HeapifyDown(heap, i);
			}
			
			return data;
		}
	}
	
	return NULL; 
}

static ssize_t HeapFind(heap_ty *heap ,heap_is_match_ty is_match ,void*  param)
{
	
	void *curr_element = NULL ; 
	size_t i = 0; 
	
	assert(heap);
	assert(is_match);
	
	for(; i < HeapSize(heap) ; ++i)
	{
		curr_element = *(void **)DVectorGetElement(heap->heap_arr , i); 
	
		if (is_match(curr_element , param) == ONE)
		{
			return i;
		}
	}
	
	return -1; 
}

static size_t HeapifyUp(heap_ty *heap, size_t ele_index)
{	
	size_t parent_index = 0;
	void **ptr_element = NULL;
	void **ptr_parent = NULL;
	
	
	if(ZERO == ele_index)
	{
		return ZERO;
	};
	
	parent_index = GetParent(ele_index);
	ptr_element = *(void**)DVectorGetElement(heap->heap_arr, ele_index);
	ptr_parent = *(void**)DVectorGetElement(heap->heap_arr, parent_index);	

	if(heap->cmp_func(ptr_element, ptr_parent) > ZERO)
	{
		Swap(DVectorGetElement(heap->heap_arr, ele_index), DVectorGetElement(heap->heap_arr, parent_index));
		return HeapifyUp(heap, parent_index);
		
	}
	
	return ele_index;
}

static size_t HeapifyDown(heap_ty *heap , size_t idx)
{
	size_t left_idx  = L_CHILD_INDEX(idx);
	size_t right_idx = R_CHILD_INDEX(idx); 
	
	void **ptr_element = (void**)DVectorGetElement(heap->heap_arr, idx);
	void **ptr_first_child = NULL;
	void **ptr_second_child = NULL;
	size_t last_index = HeapSize(heap) - 1;
	
	if(left_idx > last_index || (ZERO == last_index))
	{
		return idx;
	}
	
	ptr_first_child = (void**)DVectorGetElement(heap->heap_arr, left_idx);
	
	if(right_idx <= last_index)
	{
		
		ptr_second_child = (void**)DVectorGetElement(heap->heap_arr, right_idx);
		
		if(heap->cmp_func(*ptr_first_child, *ptr_second_child) < ZERO )
		{
			left_idx = right_idx;
			ptr_first_child = ptr_second_child;
		}
	}
	
	if(heap->cmp_func(*ptr_element, *ptr_first_child) < ZERO )
	{
		Swap(ptr_element, ptr_first_child);
		
		return HeapifyDown(heap, left_idx);
	}
	
	return idx;
}

static void Swap(void **x, void **y)
{
    void *temp =NULL ;
    
    temp = *x;
    *x = *y;
    *y = temp;
}

static size_t GetParent(size_t ele_index)
{
	
	return (ele_index != 0) ? ((ele_index - 1) / 2) : ele_index;
}


