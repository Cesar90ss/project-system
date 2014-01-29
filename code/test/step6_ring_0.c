#include "syscall.h"

// machine 0

int main()
{
	int listen_sid = Listen(1); // listen on port 1;
	
	int socket_accept, socket_connect;
	
	socket_connect = Connect(1,1);
	
	Send(socket_connect, "TOTEM", 6);
	
	socket_accept = Accept(listen_sid);
	
	char buf[6];
	
	Receive(socket_accept, buf, 6, 1);
	
	PutString(" Machine 0 has the : ");
	PutString(buf); // should be totem
	PutChar('\n');
	
	return 0;
}
