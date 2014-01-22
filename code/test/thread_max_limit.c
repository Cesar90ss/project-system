#include "syscall.h"
#define NB_THREADS 80
#define NULL 0

int sem;

void *fun(void *arg)
{
	UserSemaphoreP(sem);

	PutString("Thread execution\n");
	
	return 0;
}

int main()
{
	int i;
	int tid[NB_THREADS];

	sem = UserSemaphoreCreate("a", 0);

	for(i=0;i<NB_THREADS; i++)
	{
		tid[i] = UserThreadCreate(&fun,NULL);
		if(tid[i] < 0)
		{
			PutString("Thread creation failed\n");
		}
		else
		{
			PutString("Thread creation successfull\n");
		}
	}

	for (i = 0; i < NB_THREADS; i++)
	{
		UserSemaphoreV(sem);
	}

	for(i=0;i<NB_THREADS; i++)
	{
		tid[i] = UserThreadJoin(tid[i],NULL);
	}
	return 0;
}
