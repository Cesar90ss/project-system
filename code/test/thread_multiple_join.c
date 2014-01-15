#include "syscall.h"

/**
 * Test UserThreadJoin multiple
 **/

void *fun(void* arg)
{
    int tid = (int)arg;
    int i = 0;

    for (i = 0; i < 1000; i++);

    PutString("T2 joining...\n");
    if (UserThreadJoin(tid, 0) == -2)
        PutString("ok\n");
    return 0;
}

void *fun2()
{
    int i = 0;

    for (i = 0; i < 100000; i++);
    PutString("End fun2\n");
    return 0;
}

int main()
{
	int tid1 = UserThreadCreate(&fun2, 0);
    int tid2 = UserThreadCreate(&fun, (void *)tid1);
    if (tid1 == -1 || tid2 == 1)
    {
        PutString("Error while creating thread\n");
        return -1;
    }

	PutString("T1 joining...\n");
    if (UserThreadJoin(tid1, 0) != 0)
        PutString("Error !\n");

	return 0;
}
