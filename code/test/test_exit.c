#include "syscall.h"

int main()
{
	PutString("AAA");
	Exit(0);
	PutString("BBB");
	Exit(-1);
}
