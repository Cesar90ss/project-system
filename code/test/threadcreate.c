#include "syscall.h"

/**
 * This file test :
 * - multiple thread creation
 * - UserThreadExit
 **/
#define NB_THREADS 10

void *fun(void* arg)
{
	int *i = (int*) arg;
	int j;
	for(j=0; j< *i; j++);

	UserThreadExit(0); // thread have to stop here

    PutString("Error"); //should never be reached
    return 0; //idem
}

int main()
{
    int i, j;
	for(i=0 ; i<NB_THREADS ; i++)
	{
		if(UserThreadCreate(&fun, 0)==-1)
        {
            return -1;
        }
	}

	for(j=0 ; j<1000; j++)
	{}

    PutString("Job done");
	return 0;
}
