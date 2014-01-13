#include "syscall.h"

/**
 * Test UserThreadCreate
 **/

void fun(void* arg)
{
	int i = 0;
	for(i=0; i< 1000000; i++);
	PutInt(7);
	UserThreadExit();
}

int main()
{
	
	UserThreadCreate(&fun, 0);
	PutInt(5);
	return 0;
}
