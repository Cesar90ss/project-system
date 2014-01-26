#include "syscall.h"

int main()
{
    int fd = Open("test");
    char c;

    Read(fd, &c, 1);
    PutChar(c);
    Close(fd);

    //test in a/test should contain "2"
    SetCurrentDirectory("a");
    Open("test");
    Read(fd, &c, 2);
    PutChar(c);
    Close(fd);

    //test in root should print "1"
    Open("../test");
    Read(fd, &c, 1);
    PutChar(c);
    Close(fd);

    //test in /a/test should print "2"
    SetCurrentDirectory("/");
    Open("/a/test");
    Read(fd, &c, 2);
    PutChar(c);
    Close(fd);

    PutChar('\n');

    //Close directory
    return 0;
}
