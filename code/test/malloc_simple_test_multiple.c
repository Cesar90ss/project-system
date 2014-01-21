#include "mem_alloc.c"

int main()
{
    int *c = malloc(sizeof(int) * 100);
    int i = 0;

    if (c == NULL)
    {
        PutString("Malloc returned NULL\n");
        Exit(0);
    }

    for (i = 0; i < 100; i++)
        c[i] = i;

    int s = 0;

    for (i = 0; i < 100; i++)
        s += c[i];

    PutInt(s);
    free(c);

    if (not_all_freed())
    {
        PutString("Error while freeing all heap\n");
        Exit(0);
    }

    return 0;
}
