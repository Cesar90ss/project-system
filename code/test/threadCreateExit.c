#include "syscall.h"

void fun(void* arg)
{
	PutString("Child\n");
	UserThreadExit();
	PutString("Child error\n");
}

int main()
{
	PutString("Parent start\n");
	UserThreadCreate(&fun, 0);
	int i;
	for(i=0;i<10000;i++);
	PutString("Parent finish\n");
	return 0;
}
