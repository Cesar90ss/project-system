#include "syscall.h"
#define NB_THREADS 10
#define NULL 0

void *fun(void *arg)
{
    int file = Open("test");
    if(file >= 0)
    {
        PutString("a");
    }

    int i;
    for(i=0; i<1000; i++);

    return 0;
}

void *fun2(void *arg)
{
    int *file_id = (int*) arg;
    char c = 'a';

    if(Write(*file_id, &c, 1) <= 0)
    {
        PutString("b");
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
        if(i==4 || i==7)
        {
            id[i] = UserThreadCreate(fun2, &file);
        }
        else
        {
            id[i] = UserThreadCreate(fun, &file);
        }
    }

    for(i=0;i<NB_THREADS; i++)
    {
        UserThreadJoin(id[i],NULL);
    }
    PutString("OK\n");
    return 0;
}
