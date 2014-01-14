#include "syscall.h"
#define NB_THREADS 5

void fun(void* arg)
{
	int i;
	for(i=0;i<10000;i++);
	PutString("Child finish\n");
	UserThreadExit();
}

int main()
{
	PutString("Parent start\n");
	int id[NB_THREADS];

	int i;
	for(i=0;i<NB_THREADS;i++)
	{
		id[i] = UserThreadCreate(&fun, 0);
	}

	for(i=0;i<NB_THREADS;i++)
	{
		if(UserThreadJoin(id[i], 0) == -1)
		{
			PutInt(id[i]);
			PutString(" joined failed !\n");
		}
		else
		{
			PutInt(id[i]);
			PutString(" joined successful !\n");
		}
	}

	PutString("Parent finish\n");
	return 0;
}
