#include "syscall.h"

int main()
{
	int i=600000;
	
	if (Listen(i) == -1)
	{
		PutString(" Port does not exist\n");
		return -1;
	}
	PutString("OK\n");
	
	return 0;
}