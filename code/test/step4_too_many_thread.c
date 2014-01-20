#include "syscall.h"
#define NB_THREADS 30
#define NULL 0

void *fun(void *arg)
{
	PutString("I am a thread in a process\n");
	return 0;
}

int main()
{
	char i;
	char id[NB_THREADS];
	for(i=0;i<NB_THREADS; i++)
	{
		id[(int)i] = UserThreadCreate(&fun,NULL);
		if(id[(int)i] != -2)
		{
			PutString("Thread creation successfull\n");
		}
		else
		{
			PutString("Thread creation failed\n");
		}
	}

	for(i=0;i<NB_THREADS; i++)
	{
		if(id[(int)i] != -1)
		{
			id[(int)i] = UserThreadJoin(id[(int)i],NULL);
		}
	}
	PutString("Child ending\n");
	return 0;
}
