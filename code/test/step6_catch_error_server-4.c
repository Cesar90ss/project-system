#include "libc.c"

//This test will couple with step6_catch_error_client-2

int main()
{
	int listen_sid;  
	
	PutString("Start\n");
    listen_sid = Listen(1); // Just arbitrary port
	
	PutString("Listening...\n"); 
	int i=0,j=10000;
	for(i=0;i<j;i++);

	return listen_sid;
}
