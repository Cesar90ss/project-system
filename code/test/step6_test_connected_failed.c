#include "libc.c"

#define DATA_SIZE 13 // with the '\0'
#define ACK_SIZE 8
#define NB_LOOP 10

/**
 * Simple test, connection failed
 */



int main()
{
	int connected_sid;    
	connected_sid = Connect(0,1); // we connect to the macine 0 on port 1(see step6_test_simple_listen_accept.c)
	
	if(connected_sid < 0)
	{
			PutString("Socket connection failed can not reach remote machine\n");
			return -1;
	}
	return 0;
}