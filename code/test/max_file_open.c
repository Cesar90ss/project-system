#include "syscall.h"
#define MAX_OPEN_FILES 10
#define TRY_TO_OPEN (MAX_OPEN_FILES + 2)

int main()
{
    int f[TRY_TO_OPEN];
    int i;

    SetCurrentDirectory("a");

    char c[3];
    c[1] = '\0';
    c[2] = '\0';

    for (i = 0; i < TRY_TO_OPEN; i++)
    {
        if (i < 10)
        {
            c[0] = '0' + (char)i;
        }
        else
        {
            c[0] = '1';
            c[1] = '0' + (char)(10 - i);
        }

        f[i] = Open(c);
    }

    int ret = 1;
    for (i = 0; i < TRY_TO_OPEN; i++)
    {
        if (i < MAX_OPEN_FILES && f[i] < 0)
            ret = 0;
        else if (i >= MAX_OPEN_FILES && f[i] >= 0)
            ret = 0;
    }

    for (i = 0; i < MAX_OPEN_FILES; i++)
        Close(f[i]);

    if (ret == 0)
        Exit(0);

    PutString("ok");

    return 0;
}
