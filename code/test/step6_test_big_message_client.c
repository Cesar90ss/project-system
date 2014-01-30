#include "libc.c"

#define MAX_SIZE 128


int main()
{
	char buffer [MAX_SIZE];
	PutString("Listening\n");
	int socket_id = Connect(0,1);
	
	int i;
	if(socket_id != 0)
	{
		PutString("Connect\n");
	}
	
	Receive(socket_id,buffer,MAX_SIZE,1);
	for(i=0;i<MAX_SIZE;i++)
	{
		if(buffer[i] != i)
		{
			PutString("Corrupting Message\n");
			return -1;
		}
		
	}
	int k;
	for(k=0;k<100;k++);
	PutString("Message Arrived\n");
	return 0;
	
}