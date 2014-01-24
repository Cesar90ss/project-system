#include "syscall.h"

int main()
{
	char buffer[20];
	int receive_sid = Connect(234,55555); // Just arbitrary port and machine, must fail
	int error_code;
	if ( ( error_code = Receive(receive_sid,buffer,20) ) == -1 )
	{
		PutString("Could not receive : socket is not connected\n");
		return -1;
	}
	
	PutString("Uncatched Error\n");
	
	return 0;
}
	