#include "syscall.h"

int main()
{
    ForkExec("../build/process_long_loop");

    int i;
    for (i = 0; i < 1000; i++)
        PutString("ok\n");

	PutString("ok\n");
	return 0;
}
