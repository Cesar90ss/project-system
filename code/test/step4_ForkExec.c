#include "syscall.h"

int main()
{	
    ForkExec("../build/threadcreate");
    int i;
    for(i=0;i<10000;i++);
    return 0;
}
