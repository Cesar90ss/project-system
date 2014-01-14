#include "syscall.h"

/**
 * Test UserThreadJoin error
 **/

void fun(void* arg)
{
	int *i = (int*) arg;
	int j;
	for(j=0; j< *i; j++);
    PutString("Ending thread\n");
}

int main()
{
	int arg = 1000;
	int tid = UserThreadCreate(&fun, &arg);

    if (tid == -1)
    {
        PutString("Error while creating thread\n");
        return -1;
    }

	PutString("Start joining on bad tid... ");
    if (UserThreadJoin(tid + 100, 0) == -1)
        PutString("Error !\n");
    else
        PutString("Join Successful !\n");

	return 0;
}
