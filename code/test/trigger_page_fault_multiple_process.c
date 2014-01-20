#include "syscall.h"

int main()
{
    ForkExec("../build/trigger_page_fault");

    int i;
    for (i = 0; i < 100000; i++);

	PutString("ok\n");
	return 0;
}
