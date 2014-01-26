#include "syscall.h"

int main()
{
    int fd = Open("test");
    int fd2 = Open("/a/test");
    char c;

    PutInt(fd);
    Read(fd, &c, 1);
    PutChar(c);

    PutInt(fd2);
    Read(fd2, &c, 1);

    PutChar(c);
    PutChar('\n');

    return 0;
}
