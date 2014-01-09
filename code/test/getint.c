#include "syscall.h"

/**
 * Test GetInt
 **/

void get(int n)
{
    int i;
    int num;

    for (i = 0; i < n; i++)
    {
        num = GetInt();
        PutInt(num);
        PutChar('\n');
    }
}

int main()
{
    get(10);
    return 0;
}
