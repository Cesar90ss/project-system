#include "syscall.h"

int main()
{	
    ForkExec("../build/userpages1");
    ForkExec("../build/userpages2");
    return 0;
}
