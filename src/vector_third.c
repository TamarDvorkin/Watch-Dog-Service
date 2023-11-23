#include <stdio.h> /*frintf*/
#include <stddef.h> /*size_t*/
#include <string.h> /* memcpy */
#include <stdlib.h> /* mallock realock and free*/
#include <assert.h>
#include "../include/vector_third.h"

struct vector
{
    char *vector;
    size_t capacity;
    size_t size;
    size_t element_size;
};

#define GROWTH_FACTOR 1.5 

#define VECTOR p_vector->vector
#define CAPACITY p_vector->capacity
#define SIZE p_vector->size
#define ELEM_SIZE p_vector->element_size

//#include "include/vector_third.h" 

vector *DVectorCreate(size_t element_size, size_t capacity)
{
    vector *p_vector = (vector *)malloc( sizeof(vector) ); 
    if ( NULL == p_vector )
    {
        fprintf(stderr,"Alocation Error\n");
        return NULL;
    }

    VECTOR = (char *)malloc(element_size * capacity); 
    if ( NULL == VECTOR )
    {
        fprintf(stderr,"Alocation Error\n");        
        free(p_vector);
        return NULL;
    }

    CAPACITY = capacity;
    ELEM_SIZE = element_size; 
    SIZE = 0;

    return p_vector;
}


void DVectorDestroy(vector *p_vector)
{
    free(VECTOR);
    free(p_vector); 
}


int DVectorReserve(vector *p_vector, size_t new_cap)
{

    char *tmp = NULL;
    if(new_cap < CAPACITY)
    {
        return 0;
    }

    tmp = (char *)realloc(VECTOR , ELEM_SIZE * new_cap );
    if (NULL == tmp)
    {
        fprintf(stderr,"Realocation Error\n");
        return 1;
    }

    CAPACITY = new_cap;
    VECTOR = tmp;
    return 0;
}


void DVectorPopBack(vector *p_vector)
{
    --SIZE; 
}


int DVectorPushBack(vector *p_vector, void *elem)
{
    if (CAPACITY == SIZE)
    {
        if( DVectorReserve(p_vector, CAPACITY * GROWTH_FACTOR) )
        {
            return 1;
        }
    }

    memcpy(VECTOR + SIZE * ELEM_SIZE, elem , ELEM_SIZE);
    ++SIZE;
    return 0;
}


void *DVectorGetElement(vector *p_vector, size_t index)
{
    return VECTOR + (ELEM_SIZE * index); 
}



size_t DVectorSize(vector *p_vector)
{
    return SIZE;
}



size_t DVectorCapacity(vector *p_vector)
{
    return CAPACITY; 
}


int DVectorShrinkToFit(vector *p_vector)
{
    char *tmp = (char *)realloc(VECTOR , ELEM_SIZE * SIZE );

    if (NULL == tmp)
    {
        fprintf(stderr,"Realocation Error\n");
        return 1;
    }

    CAPACITY = SIZE;
    VECTOR = tmp;
    return 0;
}




void DVectorPrint(vector *p_vector)
{
    size_t i = 0;
    
    for(i = 0; i < DVectorSize(p_vector); ++i)
    {
        printf(" %d, ",*(int *)DVectorGetElement(p_vector, i));
    }
    
    printf("\n");
}


