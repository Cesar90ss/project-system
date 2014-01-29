#include "syscall.h"

int main()
{
    ForkExec("run2");

    int i;
    for (i = 0; i < 100000; i++);

	PutString("ok\n");
	return 0;
}
