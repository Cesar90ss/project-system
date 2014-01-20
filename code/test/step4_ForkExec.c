#include "syscall.h"

int main()
{
    ForkExec("../build/step4_ForkExec_helloworld");
    ForkExec("../build/step4_ForkExec_helloworld");
    return 0;
}
