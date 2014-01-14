#include "syscall.h"

void fun(void* arg)
{
	PutString("Child start\n");
	PutInt(*(int*)arg);
	PutString("\nChild finish\n");
	UserThreadExit(0);
}

int main()
{
	PutString("Parent start\n");
	int x = 42;
	UserThreadCreate(&fun, &x);
	int i;
	for(i=0;i<10000;i++);
	PutString("Parent finish\n");
	return 0;
}
