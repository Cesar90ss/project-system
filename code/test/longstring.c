#include "syscall.h"
#define MAX_STRING_SIZE 200

int main()
{
    int i;
    char buffer[MAX_STRING_SIZE+1];

    buffer[MAX_STRING_SIZE] = '\0';

    for (i = 0; i < MAX_STRING_SIZE; i++)
        buffer[i] = 'a';

    PutString(buffer);

    buffer[MAX_STRING_SIZE] = '\0';

    PutChar('\n');
    for (i = 0; i < MAX_STRING_SIZE; i++)
        buffer[i] = 'b';

    PutString(buffer);
    PutChar('\n');

    return 0;
}

