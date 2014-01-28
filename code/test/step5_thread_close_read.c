#include "syscall.h"
#define NB_THREADS 9
#define NULL 0

void *fun(void *arg)
{
    int file = Open("test");

    /*PutString("open : ");*/
    /*PutInt(file);*/
    /*PutChar('\n');*/

    char buffer[5];
    int i, j;

    for(i=0; i<10; i++)
    {
        Read(file, buffer, 5);
        for(j=0; j<5; j++)
        {
            //PutChar(buffer[j]);
            if(buffer[j] != '0'+i)
            {
              Exit(0);
            }
        }
    }

    for(i=0; i<1000; i++);
    return 0;
}

int main()
{
    Open("test");
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
