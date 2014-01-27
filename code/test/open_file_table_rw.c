#include "syscall.h"

int main()
{
    int f1 = Open("1");
    int f2 = Open("2");
    int f3 = Open("3");
    int f4 = Open("4");
    int f5 = Open("5");
    int f6 = Open("6");
    char c;

    Read(f1, &c, 1);
    PutChar(c);
    Read(f2, &c, 1);
    PutChar(c);
    Read(f3, &c, 1);
    PutChar(c);
    Read(f4, &c, 1);
    PutChar(c);
    Read(f5, &c, 1);
    PutChar(c);
    Read(f6, &c, 1);
    PutChar(c);

    c = '7';
    Write(f1, &c, 1);
    Read(f1, &c, 1);
    PutChar(c);
    Close(f1);

    c = '8';
    Write(f2, &c, 1);
    Read(f2, &c, 1);
    PutChar(c);
    Close(f2);

    c = '9';
    Write(f3, &c, 1);
    Read(f3, &c, 1);
    PutChar(c);
    Close(f3);

    c = '1';
    Write(f4, &c, 1);
    Read(f4, &c, 1);
    PutChar(c);
    Close(f4);

    c = '2';
    Write(f5, &c, 1);
    Read(f5, &c, 1);
    PutChar(c);
    Close(f5);

    c = '3';
    Write(f6, &c, 1);
    Read(f6, &c, 1);
    PutChar(c);
    Close(f6);

    return 0;
}
