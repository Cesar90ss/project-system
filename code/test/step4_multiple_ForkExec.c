#include "syscall.h"

int main()
{
	int i;
	for(i=0;i<15;i++)
	{
		ForkExec("../build/step4_wait");
	}

	PutString("Parent ending \n");

	return 0;
}
