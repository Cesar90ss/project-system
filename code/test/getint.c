#include "syscall.h"

/**
 * Test GetInt
 **/

void get(int n)
{
    int i;
    int num;
    int error_code;

    for (i = 0; i < n; i++)
    {
        error_code = GetInt(&num);
        if(error_code == 0){
            PutInt(num);
            PutChar(' ');
        }
        else if(error_code == -1){
            PutChar('1');
        }
        else{
            PutChar('2');
        }
    }
}

int main()
{
    get(10);
    return 0;
}
