#include "syscall.h"
#define NB_THREADS 9
#define NULL 0

int file;
void *fun(void *arg)
{
    int letter = (int) arg;

    char buffer[300];
    int i, j;

    for(j=0; j<300; j++)
    {
        buffer[j]='a' + letter;
    }

    Write(file, buffer, 300);

    for(i=0; i<1000; i++);

    return 0;
}

int main()
{
    file = Open("test");
    char id[NB_THREADS];

    int i;
    for(i=0;i<NB_THREADS; i++)
    {
        id[i] = UserThreadCreate(fun,(void*) i);
    }

    for(i=0;i<NB_THREADS; i++)
    {
        UserThreadJoin(id[i],NULL);
    }
    PutString("END\n");
    Close(file);
    return 0;
}
