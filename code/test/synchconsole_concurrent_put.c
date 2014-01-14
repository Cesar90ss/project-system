#include "syscall.h"

/**
 * Test GetString concurrently
 **/

void fn()
{
    int i = 0;
    char c[12];

    for (i = 0; i < 1000; i++)
    {
        GetString(c, 11);
        c[10] = '\n';
        PutString(c);
    }

    UserThreadExit(0);
}

int main()
{
    int tid = UserThreadCreate(fn, 0);
    int i = 0;
    char c[12];

    for (i = 0; i < 1000; i++)
    {
        GetString(c, 11);
        c[10] = '\n';
        PutString(c);
    }

    UserThreadJoin(tid, 0);

    return 0;
}
