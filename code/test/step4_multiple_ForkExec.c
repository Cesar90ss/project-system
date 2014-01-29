#include "syscall.h"
#define NB_PROC 12

int main()
{
	char i;
	for(i=0;i<NB_PROC;i++)
	{
		ForkExec("run2");
	}
	PutString("Parent ending\n");
	return 0;
}
