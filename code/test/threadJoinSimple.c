#include "syscall.h"

void fun(void* arg)
{
	int i;
	for(i=0;i<10000;i++);
	UserThreadExit(0);
}

int main()
{
	PutString("Parent start\n");
	int tid = UserThreadCreate(&fun, 0);
	if(UserThreadJoin(tid, 0) == -1)
	{
		PutString("joined failed !\n");
	}
	else
	{
		PutString("joined successful\n");
	}
	PutString("Parent finish\n");
	return 0;
}
