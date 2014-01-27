#include "syscall.h"
#define NB_PROC 2

int main()
{
	char i;
	for(i=0;i<NB_PROC;i++)
	{
		Waitpid(ForkExec("run2"), 0);
	}
	return 0;
}
