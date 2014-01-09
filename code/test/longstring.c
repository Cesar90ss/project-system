#include "syscall.h"
#define MAX_STRING_SIZE 1024

int main()
{
    int i;
    char buffer[MAX_STRING_SIZE];

    buffer[MAX_STRING_SIZE - 1] = '\0';

    for (i = 0; i < MAX_STRING_SIZE; i++)
        buffer[i] = 'a';

    PutString(buffer);

    return 0;
}

