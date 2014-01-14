#include "syscall.h"

/**
 * Test PutString concurrently
 **/

void fn()
{
    int i = 0;
    for (i = 0; i < 100; i++)
        PutString("abcdefgijklmnopqrstuvwxyz\n");
    UserThreadExit(0);
}

int main()
{
    int tid = UserThreadCreate(fn, 0);
    int i = 0;

    for (i = 0; i < 100; i++)
        PutString("1234567890ABCDEF\n");

    UserThreadJoin(tid, 0);

    return 0;
}
