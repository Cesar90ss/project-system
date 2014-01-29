#include "libc.c"

#define DATA_SIZE 13 // with the '\0'
#define NB_LOOP 10
#define ACK_SIZE 8

/**
 * Simple test, listen and then accept a connection. Then send a packet and wait for a response ten times
 * Follows the Action 2 of the subject. (Largely inspired by nettest.cc)
 */

/**
 * int Listen(int local_port);
 * int Accept(int listening_socket);
 * int Connect(int remote_machine, int remote_port);
 * int Send(char* buffer, unsigned int size);
 * int Receive(char* buffer, unsigned int size);
 * int Disconnect(int socket);
 **/


int main()
{
	int listen_sid;
	int connected_sid;
	int error_code;
	const char *data = "Hello there!";
    const char *ack = "Got it!";
    char buffer[20]; // buffer used for receive
    

	PutString("Start\n");
    listen_sid = Listen(1); // Just arbitrary port
	PutString("Listening...\n"); 
	connected_sid = Accept(listen_sid);
	if(connected_sid < 0)
	{
		PutString("Message Apocalyptique :  TO BE DEFINED\n");
		return -3;
	}
	PutString("Connected\n");
	
	int i;
	for( i=0; i< NB_LOOP; i++ )
	{
		if ( ( error_code = Send(connected_sid,(char *)data,DATA_SIZE) ) == -2 )
		{
			PutString("Could not send : Transmission problem\n");
			return -2;
		}
		if( error_code == -1 )
		{
			PutString("Error : Socket is not connected \n");
			return -1;
		}
		
		// Data is sent
		
		if ( ( error_code = Receive(connected_sid,buffer,ACK_SIZE) ) == -2 )
		{
			PutString("Could not receive : socket is waiting(listening socket)\n");
			return -2;
		}
		if( error_code == -1 )
		{
			PutString("Error : Socket is not connected \n");
			return -1;
		}
		
		// We received the message
		PutString("Received message : ");
		PutString(buffer);
		PutString("\n");
		
		if( !strcmp(buffer,(char*)ack) )
		{
			PutString(" Erro : The data is corrupted(not the one that was expected\n");
			return -4;
		}
		
	}
	
	return 0;
	
	
}
