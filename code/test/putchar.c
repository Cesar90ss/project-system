#include "syscall.h"

/**
 * Provided code from step 2
 **/

// For now define PutChar to compile (will be syscall later)
#define PutChar(x)
void print(char c, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        PutChar(c+i);
    }

    PutChar('\n');
}

int main()
{
    print('a',4);
    return 0;
}
