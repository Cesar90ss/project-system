#include "syscall.h"

int main()
{
    int fd = Open("test");
    char c;
    char cur_dir[100];

    Read(fd, &c, 1);
    PutChar(c);
    Close(fd);

    PutString(GetCurrentDirectory(cur_dir));
    SetCurrentDirectory("a");
    PutString(GetCurrentDirectory(cur_dir));

    Open("test");
    Read(fd, &c, 1);
    PutChar(c);
    Close(fd);
    PutChar('\n');

    return 0;
}
