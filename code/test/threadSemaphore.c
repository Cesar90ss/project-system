#include "syscall.h"
#define NB_THREADS 6
#define NB_MESSAGE 10

void* fun(void* arg)
{
	int i;
	for(i=0;i<NB_MESSAGE;i++)
	{
		UserSemaphoreP(*(int*)arg);
		PutChar('A');
		PutChar('B');
		PutChar('C');
		PutChar('D');
		PutChar('E');
		PutChar('\n');
		UserSemaphoreV(*(int*)arg);
	}

	return 0;
}

int main()
{
	int sem = UserSemaphoreCreate("sem",1);
	int tid[10];
	int i;
	for(i=0;i<NB_THREADS;i++)
	{
		tid[i] = UserThreadCreate(&fun, &sem);
	}

	for(i=0;i<NB_THREADS;i++)
	{
		UserThreadJoin(tid[i],0);
	}

	return 0;
}
