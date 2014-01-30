#include "libc.c"

#define MAX_SIZE 128


int main()
{
	char buffer [MAX_SIZE];
	int listening_id = Listen(1);
	if(listening_id != 0)
	PutString("Start\n");
	int socket_id = Accept(listening_id);
	PutString("Connect\n");
	int i;
	for(i=0;i<MAX_SIZE;i++)
	{
		buffer[i]=i;
	}
	int k;
	for(k=0;k<1000;k++);
	Send(socket_id,buffer,MAX_SIZE);
	return 0;
	
}