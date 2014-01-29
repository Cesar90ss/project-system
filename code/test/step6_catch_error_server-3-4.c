#include "libc.c"

#define DATA_SIZE 13 // with the '\0'
#define ACK_SIZE 8
#define NB_LOOP 10

/**
 * This test will return 6 type of error 
 * trigerred by the client the type of 
 * the error is -1 if socket does not exist
 * -2 for socket which is not listening
 * -3 if no free slot
 * -4 if the connection is already exist
 * -5 if the ack is not resceive by the client
 * -6 if the ack timeout
 */
//This test will couple with step6_catch_error_client-1

int main()
{
	int listen_sid;
	int socket_id;   

	PutString("Start\n");
    listen_sid = Listen(1); // Just arbitrary port

	PutString("Listening...\n"); 
	int i=0,j=11;
	for(i=0;i<j;i++)
	{
		socket_id = Accept(listen_sid);
        if(socket_id < 0)
        {
            switch(socket_id)
            {
			case -1:
				PutString("Socket Does Not Exist\n");
				break;
			case -2:
				PutString("No Socket Listening\n");
				break;
			case -3:
				PutString("No More Free Socket\n");
				break;
			case -4:
				PutString("The Connection Already Exist\n");
				break;
			case -5:
				PutString("The Acknoledgement was not received by the client\n");
				break;
			case -6:
				PutString("The Connection Timeout\n");
				break;
            }
        }
    }

    return 0;
}
