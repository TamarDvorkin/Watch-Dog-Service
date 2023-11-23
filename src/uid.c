

#include <stdio.h>
#include <time.h>/*time*/
#include <unistd.h> 	/*getpid*/
#include <sys/types.h>	/*pid_t*/
#include <pthread.h> /*for pthread_t*/
#include <stdatomic.h> /*atomic*/
#include "../include/uid.h"


const UID_t BadUID= {-1,0,0}; 


UID_t UidCreate(void)
{
	/*static int counter = 0;*/
	
	UID_t uid = BadUID;
	
	uid.pid = getpid();
	time(&uid.timestamp);
	/*uid.counter_uid = ++counter;*/
	uid.counter_uid = atomic_fetch_add(&uid.counter_uid, 1);

	return  uid;

}


int UidIsSame(const UID_t uid1, const UID_t uid2)
{
	/*return ((uid1.pid == uid2.pid) && (uid1.timestamp == uid2.timestamp) && (uid1.counter_uid == uid2.counter_uid));*/
	if ((uid1.pid == uid2.pid) && (uid1.timestamp == uid2.timestamp) && (uid1.counter_uid == uid2.counter_uid))
	{
	
		return 0;
	}
	return 1;

}




