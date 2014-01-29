#include "syscall.h"

int main()
{
    int ret = ForkExec("run");

    if(ret < 0)
    {
        PutString("Error\n");
        return -1;
    }
    else
    {
        PutString("Hello Myself\n");
    }

    Waitpid(ret, 0);
    return 0;
}
