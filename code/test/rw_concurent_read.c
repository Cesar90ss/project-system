#include "syscall.h"
#define NB_THREADS 9
#define NULL 0

int fid;
void *fun(void *arg)
{
    char buffer[301];
    int i, j;

    Read(fid, buffer, 300);

    for(j=0; j < 300; j++)
    {
        if(buffer[j] != buffer[0])
        {
            PutChar(buffer[j]);
            PutChar(buffer[0]);
            Exit(0);
        }
    }

    for(i=0; i<1000; i++);

    return 0;
}

int main()
{
    fid = Open("test");

    char id[NB_THREADS];

    int i;
    for(i=0;i<NB_THREADS; i++)
    {
        id[i] = UserThreadCreate(fun, NULL);
    }

    for(i=0;i<NB_THREADS; i++)
    {
        UserThreadJoin(id[i],NULL);
    }

    PutString("END\n");
    return 0;
}
