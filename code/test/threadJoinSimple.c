#include "syscall.h"

void *fun(void* arg)
{
	int i;
	for(i=0; i<10; i++){
        PutInt(i);
    }
	UserThreadExit(0);
    return 0;
}

int main()
{
	PutString("Parent start\n");
	int tid = UserThreadCreate(&fun, 0);

    if(tid == -1)
    {
        PutString("Error while creating thread\n");
        return -1;
    }

	if(UserThreadJoin(tid, 0) == -1)
	{
		PutString("joined failed !\n");
        return -1;
	}
	else
	{
		PutString("joined successfull\n");
	}
	PutString("Parent finish\n");
	return 0;
}
