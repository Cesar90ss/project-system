#include "syscall.h"

/**
 * Test simple getchar
 **/

void get(int n)
{
    int i;
    char c;
    int e;
    
    for (i = 0; i < n; i++)
    {
        e = GetChar();
        if (e == -1)
            break;

        c = (char)e;
        PutChar(c);
    }

    PutChar('\n');
}

int main()
{
    get(10);

    return 0;
}
