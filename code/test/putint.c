#include "syscall.h"

/**
 * Test PutInt
 **/

int main()
{
    int i;
    for (i = -5; i <= 5; i++)
    {
        PutInt(i);
        PutChar(' ');
    }
    return 0;
}
