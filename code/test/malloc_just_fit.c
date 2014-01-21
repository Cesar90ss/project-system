#include "mem_alloc.c"

int main()
{
    int *c = malloc(MEMORY_SIZE / 2);

    if (c == NULL)
    {
        PutString("Error\n");
        Exit(0);
    }

    c = malloc(MEMORY_SIZE / 2 - 8);
    if (c != NULL)
        PutString("ok\n");

    return 0;
}
