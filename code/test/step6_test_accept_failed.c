#include "libc.c"

#define DATA_SIZE 13 // with the '\0'
#define ACK_SIZE 8
#define NB_LOOP 10

/**
 * Simple test, listen and then accept a connection. Then send a packet and wait for a response ten times
 * Follows the Action 2 of the subject. (Largely inspired by nettest.cc)
 */



int main()
{
	int connected_sid;

	connected_sid = Accept(0);
	if(connected_sid < 0)
	{
		PutString("No listening socket, can not accept anything\n");
		return -1;
	}
	return 0;	
}