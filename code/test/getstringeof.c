#include "syscall.h"



int strlen(const char* s)
{
    int i = 0;

    while (s[i] != '\0')
        i++;

    return i;
}

/**
 * Test GetString
 **/
#define BUF_SIZE 5
void get(int n)
{
    int i;
    char buffer[BUF_SIZE + 1];

    buffer[BUF_SIZE] = '\0';

    for (i = 0; i < n; i++)
    {
        if(GetString(buffer, BUF_SIZE + 1)== 0)
	{
	  PutString("End Of File");
	}
        PutString(buffer);
        PutChar('\n');

        if (strlen(buffer) != BUF_SIZE)
            return;
    }
}

int main()
{
    get(10);

    return 0;
}
