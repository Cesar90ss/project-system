#include "syscall.h"

void g()
{
    UserThreadExit((void *)42);
}

void *fun(void* arg)
{
    g();
    return 0;
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
