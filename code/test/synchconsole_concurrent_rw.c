#include "syscall.h"

/**
 * Test GetString/PutString same time
 * Need to launch & wait before typding 2 char
 **/

void fn()
{
    char c = GetChar();
    c = c;
    UserThreadExit(0);
}

int main()
{
    int tid = UserThreadCreate(fn, 0);
    int i = 0;

    for (i = 0; i < 1000; i++)
        PutString("1234567890ABCDEF\n");

    UserThreadJoin(tid, 0);

    return 0;
}
