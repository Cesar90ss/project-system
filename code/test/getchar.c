#include "syscall.h"

/**
 * Test simple getchar
 **/

void get(int n)
{
    int i;
    char c;
    for (i = 0; i < n; i++)
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
