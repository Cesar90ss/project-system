#include "syscall.h"
#define NB_THREADS 40
#define NULL 0

void *fun(void *arg)
{
	PutString("Thread execution\n");
	int i;
	for(i=0;i<10;i++);
	return 0;
}

int main()
{
	PutString("Child start\n");
	int i;
	int tid[NB_THREADS];

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

	for(i=0;i<NB_THREADS; i++)
	{
		tid[i] = UserThreadJoin(tid[i],NULL);
	}
	
	PutString("Child end\n");
	return 0;
}
