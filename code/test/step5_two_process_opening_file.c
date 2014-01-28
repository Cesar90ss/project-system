#include "syscall.h"
#define NB_THREADS 2
#define NULL 0

int main()
{
    int pid1;
    int pid2;

    pid1 = ForkExec("run2");
    pid2 = ForkExec("run2");

    return 0;
}
