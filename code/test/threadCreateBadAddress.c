#include "syscall.h"

int main()
{
	PutString("Parent start\n");
	UserThreadCreate((void*)-1, 0);
	int i;
	for(i=0;i<10000;i++);
	PutString("Parent finish\n");
	return 0;
}
