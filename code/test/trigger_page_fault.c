#include "syscall.h"
#define NULL 0

/**
 * Write at addr 0
 * There is code readOnly
 **/
int main()
{
    int *a;

    a = (int *)NULL;

    *a = 42;

    return 0;
}
