#include "syscall.h"

int main()
{
    ForkExec("run1");
    ForkExec("run2");
    return 0;
}
