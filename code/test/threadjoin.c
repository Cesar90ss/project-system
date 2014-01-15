#include "syscall.h"

/**
 * Test UserThreadJoin
 **/

void *fun(void* arg)
{
	int *i = (int*) arg;
	int j;
	for(j=0; j< *i; j++);

    PutString("Ending thread\n");
    UserThreadExit(0);
    return 0;
}

int main()
{
	int arg = 100000;
	int tid = UserThreadCreate(&fun, &arg);

    if (tid == -1)
    {
        PutString("Error while creating thread\n");
        return -1;
    }

	PutString("Start joining...\n");

    if (UserThreadJoin(tid, 0) == -1)
        PutString("Error !\n");
    else
        PutString("Join Successful !\n");

	return 0;
}
