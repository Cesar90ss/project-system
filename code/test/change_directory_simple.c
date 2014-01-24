#include "syscall.h"

int main()
{
    int fd = Open("test");
    char c;
    char cur_dir[100];

    Read(&c, 1, fd);
    PutChar(c);
    Close(fd);

    PutString(GetCurrentDirectory(cur_dir));
    SetCurrentDirectory("a");
    PutString(GetCurrentDirectory(cur_dir));

    Open("test");
    Read(&c, 1, fd);
    PutChar(c);
    Close(fd);
    PutChar('\n');

    return 0;
}
