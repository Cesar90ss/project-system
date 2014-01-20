#include "syscall.h"

/**
 * Just a long loop
 **/
int main()
{
    int i;

    for (i = 0; i < 10000; i++);

    PutString("done\n");
    
    return 0;
}
