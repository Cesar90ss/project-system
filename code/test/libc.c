/**
  * All usefull utils here
  **/

#include "syscall.h"
#define NULL 0

typedef unsigned int size_t;

/**
 * Simulate memcpy C
 **/
void *memcpy(void *dest, const void *src, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++)
        ((char*)dest)[i] = ((char*)src)[i];

    return dest;
}

/**
 * Simulate memset C
 **/
void *memset(void *s, int c, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++)
        ((char*)s)[i] = (char)c;

    return s;
}
