#include "syscall.h"
#define EOF -1

/**
 * Test simple getchar
 **/

void get(int n)
{

    int c=2;
    
    while (c!=EOF)
    {
        c = GetChar();
        PutChar(c);
    }

    PutChar('\n');
}

int main()
{
    get(10);

    return 0;
}
