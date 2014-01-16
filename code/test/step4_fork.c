#include "syscall.h"

int main()
{	
    if(Fork())
	{
		PutString("I'm the parent\n");
	}
	else
	{
		PutString("I'm the child\n");
	}
	return 0;
}
