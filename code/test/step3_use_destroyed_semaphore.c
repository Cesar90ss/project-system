#include "syscall.h"


int main()
{
	int sem = UserSemaphoreCreate("sem",1);// create semaphore
	UserSemaphoreDestroy(sem); //destroy main thread
	if((UserSemaphoreP(sem) == -1) || (UserSemaphoreV(sem) == -1))
	{
	    PutString("error \n");
	}
	return 0;
}
