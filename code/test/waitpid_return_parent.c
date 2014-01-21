#include "syscall.h"

int main()
{
    unsigned int pid;
    int val;

    pid = ForkExec("waitpid_return");
    if (pid < 0)
    {
        PutString("Error while Fork\n");
        return -1;
    }

    Waitpid(pid, &val);

    PutString("Child exit code was ");
    PutInt(val);
    PutChar('\n');

    return 0;
}
