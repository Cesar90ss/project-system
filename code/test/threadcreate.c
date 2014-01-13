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
	UserThreadCreate(&fun, 0);
	PutInt(5);
	return 0;
}
