#include "syscall.h"

int main()
{
    UserThreadJoin(0,0);
    PutString("I can not wait for myself ");
    return 0;
}