#include "libc.c"

#define MAX_SIZE 128


int main()
{
	char buffer [MAX_SIZE];
	int socket_id = Connect(0,1);
	
	int i;
	if(socket_id != 0)
	{
		PutString("Connect");
	}
	for(i=0;i<MAX_SIZE;i++)
	{
		buffer[i]=i;
	}
	
	return Send(socket_id,buffer,MAX_SIZE);
	
}