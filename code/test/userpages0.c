#include "syscall.h"
#define NB_THREADS 3

void* fun()
{
    int j;
    char c = 'a';
    for(j=0; j<9; j++)
    {
        PutChar(c+j);
    }

    for (j= 0; j < 10000; j++);
    return 0;
}

int main()
{
    int id[NB_THREADS];

    int i;
    for(i=0;i<NB_THREADS;i++)
    {
        id[i] = UserThreadCreate(&fun, 0);
        //Some check for the threads creation
        if(id[i] < 0)
        {
            PutString("Error while creating thread\n");
            // Wait for putstring
            UserThreadJoin(id[i]-1, 0);
            return -1;
        }
    }

    for(i=0;i<NB_THREADS;i++)
    {
        //Joining all threads
        if(UserThreadJoin(id[i], 0) == -1)
        {
            PutString(" joined failed! ");
        }
    }

    PutString("\nParent finish\n");
    return 0;
}

