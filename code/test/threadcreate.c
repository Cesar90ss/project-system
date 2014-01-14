#include "syscall.h"

/**
 * Test UserThreadCreate
 **/

void fun(void* arg)
{
	PutInt(7);
	int *i = (int*) arg;
	//for(i=0; i< 100; i++);
	PutInt(*i);
	UserThreadExit();
}

int main()
{
    int i;

	UserThreadCreate(&fun, 0);
	PutInt(5);
    for (i = 1; i < 10000; i++);

int *arg = (int *)544;
	*arg = 26;
	UserThreadCreate(&fun, arg);
	PutInt((int)arg);
	PutInt(9);
	return 0;
}
