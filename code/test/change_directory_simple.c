#include "syscall.h"
#include "mem_alloc.c"

int main()
{
    int fd = Open("test");
    char c;
    char *cur_dir = malloc(100);

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

    free(cur_dir);
    return 0;
}
