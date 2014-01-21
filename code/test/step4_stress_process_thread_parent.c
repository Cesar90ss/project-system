#include "syscall.h"
#define NB_PROC 150

int main()
{
	int i;
	for(i=0;i<NB_PROC;i++)
	{
		if(ForkExec("../build/step4_stress_process_thread_child") >= 0)
			PutString("Process Creation successfull\n");
		else
			PutString("Process creation failed\n");
	}
	PutString("Parent end\n");
	return 0;
}
