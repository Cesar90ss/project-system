#include "syscall.h"

/**
 * Test GetInt
 **/

void get(int n)
{
    int i;
    int num;
    int error_code;

    for (i = 0; i <= n; i++)
    {
        error_code = GetInt(&num);
        if(error_code == 1)
        {
            PutInt(num);
            PutChar(' ');
        }
        else if(error_code == -1)
        {
            PutString("EOF");
        }
        else if(error_code == -2)
        {
            PutString("Bad addr\n");
        }
        else
        {
            PutString("NaN\n");
        }
    }
}

int main()
{
    get(10);
    return 0;
}
