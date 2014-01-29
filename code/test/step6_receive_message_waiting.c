#include "syscall.h"

int main()
{
	char buffer[20];
	int listen_sid = Listen(1); // Just arbitrary port 
	int error_code = listen_sid; // make gcc happy
	if ( ( error_code = Receive(listen_sid,buffer,20,1) ) == -2 )
	{
		PutString("Could not receive : socket is waiting(listening socket)\n");
		return -2;
	}
	
	PutString("Uncatched Error\n");
	
	return 0;
}
	
