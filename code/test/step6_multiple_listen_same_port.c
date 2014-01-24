#include "syscall.h"

int main()
{
	Listen(1);
	
	if (Listen(1) != -1)
	{
		PutString(" Error, did not catch multiple listen on same port\n");
		return -1;
	}
	PutString("OK\n");
	
	return 0;
}