#include "mem_alloc.c"

int main()
{
    void* c[4];

    PutString("1\n");
    c[0] = malloc(42);          /* 1 */

    PutString("2\n");
    c[1] = malloc(100);         /* 2 */

    PutString("3\n");
    c[2] = malloc(225);         /* 3 */

    PutString("4\n");
    free(c[0]);                 /* -1 */

    PutString("5\n");
    c[0] = malloc(1);           /* 1 */

    PutString("6\n");
    c[3] = malloc(110);         /* 4 */

    PutString("7\n");
    free(c[1]);                 /* -2 */
    PutString("8\n");

    free(c[2]);                 /* -3 */
    PutString("9\n");

    c[1] = malloc(42);          /* 2 */
    PutString("10\n");

    free(c[0]);                 /* -1 */
    PutString("11\n");

    free(c[3]);                 /* -4 */
    PutString("12\n");

    free(c[1]);                 /* -2 */
    PutString("13\n");

    if (not_all_freed())
    {
        PutString("Error while freeing all heap\n");
        Exit(0);
    }

    return 0;
}
