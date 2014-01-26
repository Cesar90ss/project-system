#include "syscall.h"

int main()
{
    Create("42");
    PutInt(Unlink("42"));

    return 0;
}
