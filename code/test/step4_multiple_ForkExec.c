#include "syscall.h"
#define NB_PROC 9

int main()
{
	char i;
	for(i=0;i<NB_PROC;i++)
	{
		ForkExec("../build/step4_multiple_Thread_Process");
	}
	PutString("Parent ending\n");
	return 0;
}
