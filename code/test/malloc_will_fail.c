#include "mem_alloc.c"

int main()
{
    int *c = malloc(MEMORY_SIZE - 4);

    if (c == NULL)
    {
        PutString("Error\n");
        Exit(0);
    }

    c = malloc(1);
    if (c == NULL)
        PutString("ok\n");

    return 0;
}
