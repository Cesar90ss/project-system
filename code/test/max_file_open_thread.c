#include "syscall.h"
#define MAX_OPEN_FILES 5

void *fn(void * arg)
{
    int f[MAX_OPEN_FILES];
    int i;


    char c[3];
    c[1] = '\0';
    c[2] = '\0';

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (i < 10)
        {
            c[0] = '0' + (char)(i + (int)arg*5);
        }
        else
        {
            c[0] = '1';
            c[1] = '0' + (char)(10 - (char)(i + (int)arg*5));
        }

        f[i] = Open(c);
        PutInt(f[i]);
    }

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (f[i] < 0)
            Exit(0);
    }

    return 0;
}

int main()
{
    SetCurrentDirectory("a");

    int t1 =UserThreadCreate(fn, 0);
    int t2 =UserThreadCreate(fn, (void *)1);
    UserThreadJoin(t1, 0);
    UserThreadJoin(t2, 0);

    int f1 = Open("/b/1");
    int f2 = Open("/b/2");

    if (f1 >= 0)
        Exit(0);

    if (f2 >= 0)
        Exit(0);

    PutString("ok");

    return 0;
}
