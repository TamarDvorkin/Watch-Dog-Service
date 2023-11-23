
#ifndef _VECTOR_H_
#define _VECTOR_H_


typedef struct vector vector;


/*******************************************************************************
*  Description: create vector
*  Errors:   2 malloc error 
*  Parameters: size_of_elements, capacity
*  Return value:   vector pointer 
*  Complexity:     O(1) + malloc
*******************************************************************************/
 vector *DVectorCreate(size_t element_size, size_t capacity);


/*******************************************************************************
*  Description: destroy the vector
*  Errors:     ---     
*  Parameters: pointer to vector
*  Return value: --   
*  Complexity:  O(1) +free
*******************************************************************************/
void DVectorDestroy(vector *p_vector);


/*******************************************************************************
*  Description: increase the vector capacity
*  Errors: 
*  Parameters: pointer to vector, new capacity
*  Return value:    0-success 1- fail
*  Complexity:      O(1)+relloc 
*******************************************************************************/
int DVectorReserve(vector *p_vector, size_t new_cap);


/*******************************************************************************
*  Description: increase the vector capacity 
*  Errors:  ---
*  Parameters: pointer to vector
*  Return value:  ---  
*  Complexity:    O(1)+malloc 
*******************************************************************************/
void DVectorPopBack(vector *p_vector);


/*******************************************************************************
*  Description: push new element at the end of vector
*  Errors: memcpy error
*  Parameters: pointer to vector, element to push
*  Return value:   0-success 1- fail 
*  Complexity:    O(1)+relloc  
*******************************************************************************/
int DVectorPushBack(vector *p_vector, void *elem);


/*******************************************************************************
*  Description: return value of element in specific index of the vector
*  Errors:  ---
*  Parameters: pointer to vector, index
*  Return value: value of element   
*  Complexity:     O(1)
*******************************************************************************/
void *DVectorGetElement(vector *p_vector, size_t index);


/*******************************************************************************
*  Description: calculate size  of vector
*  Errors: ---
*  Parameters: pointer to vector
*  Return value:    size of vector
*  Complexity:     O(1)
*******************************************************************************/
size_t DVectorSize(vector *p_vector);


/*******************************************************************************
*  Description: returns the capacity of vector
*  Errors: ---
*  Parameters: pointer to vector
*  Return value:  current capacity of vector  
*  Complexity:    O(1) 
*******************************************************************************/
size_t DVectorCapacity(vector *p_vector);


/*******************************************************************************
*  Description: Shrink the capacity to the vector's size
*  Errors: realloc
*  Parameters: pointer to vector
*  Return value:  0-success 1- fail  
*  Complexity:     O(1) +realloc
*******************************************************************************/
int DVectorShrinkToFit(vector *p_vector);

void DVectorPrint(vector *p_vector);

#endif /*_VECTOR_H_*/


