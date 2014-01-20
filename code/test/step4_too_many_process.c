#include "syscall.h"
#define NB_PROC 40

int main()
{
	char i;
	for(i=0;i<NB_PROC;i++)
	{
		if(!ForkExec("../build/step4_multiple_Thread_Process"))
		{
			PutString("Process Creation successfull\n");
		}
		else
		{
			PutString("Process creation failed\n");
		}
	}
	PutString("Parent ending\n");
	return 0;
}
