#include "syscall.h"

void *fun(void* arg)
{
	int i;

    for(i=0;i<100000;i++);

    PutString("Fin !\n");
    return 0;
}

int main()
{
    int tid = 0;

		tid = UserThreadCreate(&fun, 0);
		tid = tid; // make gcc happy
    PutString("La ");
    UserThreadExit(0);
    PutString("Mort\n");

	return 1;
}
