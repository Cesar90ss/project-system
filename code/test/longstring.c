#include "syscall.h"
#define MAX_STRING_SIZE 200

void bzero(char *a, int size)
{
    int i = 0;

    for (i = 0; i < size; i++)
        a[i] = '\0';
}

int main()
{
    int i;
    char buffer[MAX_STRING_SIZE+1];

    bzero(buffer, MAX_STRING_SIZE);

    for (i = 0; i < MAX_STRING_SIZE; i++)
        buffer[i] = 'a';

    PutString(buffer);

    bzero(buffer, MAX_STRING_SIZE);

    PutChar(' ');
    for (i = 0; i < MAX_STRING_SIZE; i++)
        buffer[i] = 'b';

    PutString(buffer);
    PutChar('\n');

    return 0;
}

