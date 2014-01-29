#include "syscall.h"

int main()
{
    int dead_pid;
    dead_pid = ForkExec("run2");
    Waitpid(dead_pid, 0);
    if(Waitpid(dead_pid, 0) == -2)
    {
        PutString("No need to wait Dead Process ");
    }
    return 0;
}
