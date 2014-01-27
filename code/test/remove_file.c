#include "syscall.h"

int main()
{
    Create("42");
    PutInt(Remove("42"));

    return 0;
}
