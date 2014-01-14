#include "syscall.h"

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
	int id[10];

	int i;
	for(i=0;i<10000;i++)
	{
		id[i] = UserThreadCreate(&fun, 0);
	}

	for(i=0;i<10000;i++)
	{
		UserThreadJoin(id[i]);
		PutInt(id[i]);
		PutString(" joined\n");
	}

	PutString("Parent finish\n");
	return 0;
}
