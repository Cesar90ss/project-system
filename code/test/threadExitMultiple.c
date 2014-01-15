#include "syscall.h"

void *fun(void* arg)
{
	int i;
	for(i=0;i<10000;i++);
	PutString("error\n");
	UserThreadExit(0);
    return 0;
}

void *killer(void* arg)
{
	Exit(0);
    return 0;
}

int main()
{
	PutString("Parent start\n");
	UserThreadCreate(&fun, 0);
	UserThreadCreate(&killer, 0);
	int i;
	for(i=0;i<10000;i++);
	PutString("Parent finish\n");
	return 0;
}
