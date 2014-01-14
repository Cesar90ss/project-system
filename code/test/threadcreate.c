#include "syscall.h"

/**
 * Test UserThreadCreate
 **/

void fun(void* arg)
{
	PutInt(7);
	UserThreadExit();
}

int main()
{
    int i;

	UserThreadCreate(&fun, 0);
	PutInt(5);
    for (i = 1; i < 10000; i++);

	return 0;
}
