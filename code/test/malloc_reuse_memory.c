#include "mem_alloc.c"

int main()
{
    int *c = malloc(100 * sizeof(int));

    if (c == NULL)
    {
        PutString("Malloc returned NULL\n");
        Exit(0);
    }

    free(c);

    int *c2 = malloc(20 * sizeof(int));

    free(c2);

    if (not_all_freed())
        PutString("Error while freeing all heap\n");
    else if (c == c2)
        PutString("ok\n");

    return 0;
}
