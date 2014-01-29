#include "syscall.h"

int main()
{
    int i;

    for (i = 0; i < 10; i++)
        ForkExec("run2");

    return 0;
}
