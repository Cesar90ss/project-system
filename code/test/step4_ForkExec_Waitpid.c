#include "syscall.h"

int main()
{
    Waitpid(ForkExec("run2"), 0);
    PutString("in my world the parent should come after the child...\n");
    return 0;
}
