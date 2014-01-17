#include "syscall.h"
#define NB_THREADS 11 // maximum number of user thread can created

void* fun()
{

    int i;
    for(i=0; i<10;i ++)
    {
        PutInt(i);
    }
    return 0;
}

void* last_killed_thread()
{

    int i;
    for(i=0; i<2;i ++)
    {
        PutInt(i);
    }
    return 0;
}


int main()
{
    //PutString("Parent start\n");
    int id[NB_THREADS-1];

    int i;
    for(i=0;i<NB_THREADS-1;i++)
    {
        id[i] = UserThreadCreate(&fun, 0);

        //Some check for the threads creation
        if(id[i] == -1)
        {
            PutString("Error while creating thread\n");
            return -1;
        }
    }

    //killed 1 thread    
    UserThreadJoin(UserThreadCreate(last_killed_thread, 0),0);
    //create new other thread
    UserThreadCreate(last_killed_thread, 0);

    for(i=0;i<NB_THREADS-1;i++)
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
