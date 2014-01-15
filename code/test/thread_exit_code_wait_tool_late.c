#include "syscall.h"

void *fun(void* arg)
{
    return (void *)42;
}

int main()
{
	int tid = UserThreadCreate(&fun, 0);
    int retval;

    if(tid == -1)
    {
        PutString("Error while creating thread\n");
        return -1;
    }

    int i;
    for (i = 0; i < 10000; i++);

	if(UserThreadJoin(tid, (void **)&retval) == -1)
	{
		PutString("joined failed !\n");
        return -1;
	}
	else
	{
        PutString("Exit :");
        PutInt((int)retval);
        PutChar('\n');
	}

	return 0;
}
