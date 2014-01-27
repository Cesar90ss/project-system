#include "syscall.h"
#define NB_THREADS 2
#define NULL 0

void *fun(void *arg)
{
    int c = Open("test");
    if(c>=0)
    {
        PutString("Opening successfull");
    }
    else
    {
        PutString("Opening failed");
    }

    int i;
    for(i=0; i<1000; i++);

    Close(c);
    return 0;
}
int main()
{
    int i;
    char id[NB_THREADS];

    for(i=0;i<NB_THREADS; i++)
    {
        id[i] = UserThreadCreate(fun,NULL);
    }
    for(i=0;i<NB_THREADS; i++)
    {
        UserThreadJoin(id[i],NULL);
    }
    PutChar('\n');
    return 0;
}
