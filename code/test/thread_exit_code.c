#include "syscall.h"

void fun(void* arg)
{
	int i;

    for(i=0;i<10000;i++);

	UserThreadExit((void *)42);
}

int main()
{
    int *retCode;
    int tid = 0;

	PutString("Parent start\n");
	tid = UserThreadCreate(&fun, 0);
    UserThreadJoin(tid, (void **)(&retCode));

    PutString("Exit code of child is ");
    PutInt((int)retCode);

	PutString("\nParent finish\n");
	return 0;
}
