#include "syscall.h"
#define NB_THREADS 9
#define NULL 0

void *fun(void *arg)
{
    int file = Open("test");
    if(file >= 0)
    {
        PutString("ok");
    }

    int i;
    for(i=0; i<1000; i++);

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
    PutString("END\n");
    return 0;
}
