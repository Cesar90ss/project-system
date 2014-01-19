#include "syscall.h"
#define NB_THREADS 4

void* fun()
{
      
    PutString("String2");
    
    return 0;
}

int main()
{
    //PutString("Parent start\n");
    int id[NB_THREADS];

    int i;
    for(i=0;i<NB_THREADS;i++)
    {
        id[i] = UserThreadCreate(&fun, 0);
        //Some check for the threads creation
        if(id[i] == -1)
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
        else
        {
            PutString(" joined successfull! ");
        }
    }

    PutString("Parent finish\n");
    return 0;
}

