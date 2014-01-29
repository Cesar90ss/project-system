#include "libc.c"


/**
 * This test will return 6 type of error 
 * trigerred by the client the type of 
 * -1 if no free socket
 * -2 if the ack is not resceive by the client
 * -3 if the ack timeout
 */
//This test will couple with step6_catch_error_server-3-4

int main()
{
	int socket_id;   
	PutString("Listening...\n"); 
	int i=0,j=11;
	for(i=0;i<j;i++)
	{
	socket_id = Connect(0,1);
	}
	
	if(socket_id < 0)
	{
		switch(socket_id)
		{
			case -1:
				PutString("No More Free Socket\n");
				break;
			case -2:
				PutString("The Acknoledgement was not received by the client\n");
				break;
			case -3:
				PutString("The Connection Timeout\n");
				break;
		}
		return socket_id;
	}
	PutString("Server Accept the Connection\n");
	return 0;
}