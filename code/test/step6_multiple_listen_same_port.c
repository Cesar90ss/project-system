#include "syscall.h"

int main()
{
	Listen(1);
	
	if (Listen(1) == -2)
	{
		PutString(" The port is already used by another listener\n");
		return -2;
	}
	PutString("OK\n");
	
	return 0;
}