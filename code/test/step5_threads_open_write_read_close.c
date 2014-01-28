#include "syscall.h"
#define NB_THREADS 1
#define NULL 0

void *fun(void *arg)
{
    int *file_id = (int*) arg;
    char c = 'a';

    if(Write(*file_id, &c, 1) != 1)
    {
        PutString("Write exiting\n");
        Exit(0);
    }
    else if(Read(*file_id, &c, 1) != 0)
    {
        PutString("Read exiting\n");
        Exit(0);
    }
    else if(Close(*file_id) != 0)
    {
        PutString("Close exiting\n");
        Exit(0);
    }

    return 0;
}
int main()
{
    int i;
    int file = Open("test");
    char id[NB_THREADS];

    for(i=0;i<NB_THREADS; i++)
    {
        id[i] = UserThreadCreate(fun, &file);
    }

    for(i=0;i<NB_THREADS; i++)
    {
        UserThreadJoin(id[i],NULL);
    }
    PutString("OK\n");
    return 0;
}
