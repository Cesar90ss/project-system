#include "syscall.h"

int main()
{
    int fd = Open("test");
    char c;

    Seek(fd, 2);

    Read(fd, &c, 1);
    PutChar(c);
    Close(fd);

    PutChar('\n');

    //Close directory
    return 0;
}
