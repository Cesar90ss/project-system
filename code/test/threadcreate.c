#include "syscall.h"

/**
 * Test UserThreadCreate
 **/

void fun(void* arg)
{
	int *i = (int*) arg;
	int j;
	for(j=0; j< *i; j++);
	UserThreadExit();
}

int main()
{
	int arg = 1000;
	UserThreadCreate(&fun, &arg);
	PutString("GoodBye ");
	return 0;
}
