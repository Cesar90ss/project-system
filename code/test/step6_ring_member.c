#include "syscall.h"

// machine i

int main()
{
	int listen_sid = Listen(1); // listen on port 1;
	
	int socket_accept, socket_connect;
	
	socket_accept = Accept(listen_sid);
	
	char buf[6];
	
	Receive(socket_accept, buf, 6);
	
	PutString(" Machine 1 has the : ");
	PutString(buf); // should be totem
	PutChar('\n');
	
	socket_connect = Connect(2,1); // machine i+1
	
	Send(socket_connect, "TOTEM", 6);
	
	return 0;
}