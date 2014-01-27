#include "syscall.h"

int main()
{
    int fd = Open("est");;
    int ret = Remove("test");

    if(ret != 0)
    {
        PutString("True");
    } 

    Close(fd);
    PutChar('\n');

    //Close directory
    return 0;
}
