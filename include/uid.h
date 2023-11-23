
#ifndef _UID_H_
#define _UID_H_

#include <sys/types.h>		/*for pid_t*/

struct uid
{
	pid_t pid;
	time_t timestamp;
	size_t counter_uid;
};

typedef struct uid UID_t;

extern const UID_t BadUID;


/*******************************************************************************
*  Description:     create a new uid 
*  Errors:          
*  Parameters:      
*  Return value:    by value
*  Complexity:      
*******************************************************************************/
UID_t UidCreate(void);


/*******************************************************************************
*  Description:     commper between two uid 
*  Errors:          
*  Parameters:      
*  Return value:    0- success 1- fail
*  Complexity:      
*******************************************************************************/
int UidIsSame(const UID_t uid1, const UID_t uid2);


#endif /*_UID_H_*/




