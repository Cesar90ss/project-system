#include "syscall.h"

void fun(void* arg)
{
	PutString("Child\n");
	UserThreadExit();
}

int main()
{
	PutString("Parent start\n");
	int i;
	for(i=0;i<10;i++)
	{
		UserThreadCreate(&fun, 0);
	}
	for(i=0;i<10000;i++);
	PutString("Parent finish\n");
	return 0;
}
