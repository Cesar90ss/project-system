#include "mem_alloc.c"

int main()
{
    PutString("FirstMalloc !\n");
    int *c = malloc(100 * sizeof(int));
    int i = 0;

    if (c == NULL)
    {
        PutString("Malloc returned NULL\n");
        Exit(0);
    }

    PutString("Fill memory\n");
    for (i = 0; i < 100; i++)
        c[i] = i;

    PutString("Print memory\n");
    for (i = 0; i < 100; i++)
    {
        PutInt(c[i]);
        PutChar(' ');
    }

    PutString("\nFree !\n");
    free(c);

    if (not_all_freed())
    {
        PutString("Error while freeing all heap\n");
        Exit(0);
    }

    return 0;
}
