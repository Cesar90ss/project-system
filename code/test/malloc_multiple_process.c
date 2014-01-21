#include "syscall.h"

int main()
{
    int i;

    for (i = 0; i < 10; i++)
        ForkExec("../build/malloc_simple_test_multiple");

    return 0;
}
