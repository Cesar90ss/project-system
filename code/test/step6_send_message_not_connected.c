#include "syscall.h"

int main()
{
	char buffer[50];
	unsigned int size = 10;
	int socket = 234;
	int error_type = Send(socket,buffer, size);
	if( error_type < 0 )
	{
		if(error_type == -1)
			PutString(" Error, Socket not connected \n");
		else
			PutString(" Transmission Error \n");
		return error_type;
	}
	return 0;
}