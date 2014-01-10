#include "syscall.h"

int main()
{
	PutChar('A');
	PutChar('\n');
	Exit(0);
	PutChar('B');
	PutChar('\n');
	Exit(-1);
}
