#include "mem_alloc.c"

int main()
{
    free((void*)12);

    if (not_all_freed())
    {
        PutString("Error while freeing all heap\n");
        Exit(0);
    }

    return 0;
}
