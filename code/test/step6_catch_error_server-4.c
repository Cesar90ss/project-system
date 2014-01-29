#include "libc.c"

#define DATA_SIZE 13 // with the '\0'
#define ACK_SIZE 8
#define NB_LOOP 10


int main()
{
	int listen_sid;  
	
	PutString("Start\n");
    listen_sid = Listen(1); // Just arbitrary port
	
	PutString("Listening...\n"); 
	int i=0,j=1000000;
	for(i=0;i<j;i++);

	return listen_sid;
}