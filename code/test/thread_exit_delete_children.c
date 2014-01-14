#include "syscall.h"

void fun(void* arg)
{
    int i;

	PutString("Child\n");
    for (i = 0; i < 1000; i++);
	Exit(0);
}

int main()
{
	PutString("Parent start\n");
	int i;
    int tid;
	for(i=0;i<10;i++)
	{
		tid = UserThreadCreate(&fun, 0);
	}

    UserThreadJoin(tid, 0);
	PutString("Parent finish\n");
	return 0;
}
