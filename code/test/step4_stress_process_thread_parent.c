#include "syscall.h"
#define NB_PROC 150

int main()
{
	int i;
	for(i=0;i<NB_PROC;i++)
	{
		if(ForkExec("run2") >= 0)
			PutString("Process Creation successfull\n");
		else
			PutString("Process creation failed\n");
	}
	PutString("Parent end\n");
	return 0;
}
