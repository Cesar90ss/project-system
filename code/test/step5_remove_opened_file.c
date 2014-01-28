#include "syscall.h"

int main()
{
    int fd = Open("test");

    if(Remove("test") != 0)
    {
        PutString("Didnt removed the file");
    }
    Close(fd);
    PutChar('\n');

    //Close directory
    return 0;
}
