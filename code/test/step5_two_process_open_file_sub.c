#include "syscall.h"

int main()
{
    int c = Open("test");
    if(c>=0)
    {
        PutString("ok");
    }
    else
    {
        PutString("bad");
    }

    int i;
    for(i=0; i<10000; i++);

    Close(c);
    return 0;
}
