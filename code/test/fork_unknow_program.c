#include "syscall.h"

int main()
{
    int i;

    for (i = 0; i < 20; i++)
    {
        if (ForkExec("../build/unk0wN123456789") < 0)
            PutString("Fork failed\n");
    }

    PutString("End\n");
    return 0;
}
