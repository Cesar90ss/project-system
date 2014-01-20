#include "syscall.h"

#define HEAP_PAGE_NUMBER 512
#define PAGE_SIZE 128
int main()
{
    int heap_addr = 0;
    int new_heap_addr = 0;
    int i;

    heap_addr = AllocPageHeap();
    if (heap_addr == -1)
    {
        PutString("Error while asking for heap page\n");
        return -1;
    }

    // Test one write/read inside new heap
    *(int *)heap_addr = 42;
    PutInt(*(int *)heap_addr);

    for (i = 1; i < HEAP_PAGE_NUMBER; i++)
        heap_addr = AllocPageHeap();

    // This one should fail
    new_heap_addr = AllocPageHeap();
    if (new_heap_addr == -1)
        PutString(" ok1 ");

    heap_addr += PAGE_SIZE;
    
    // Go down one page by one
    for (i = 0; i < HEAP_PAGE_NUMBER; i++)
    {
        new_heap_addr = FreePageHeap();
        if (new_heap_addr + PAGE_SIZE != heap_addr)
        {
            PutString("new_heap_addr = ");
            PutInt(new_heap_addr);
            PutString(" heap_addr = ");
            PutInt(heap_addr);
            PutString("\n");
            return -1;
        }

        heap_addr = new_heap_addr;
    }

    PutString("ok2 ");

    // Should not change current pointer
    new_heap_addr = FreePageHeap();
    if (new_heap_addr == heap_addr)
        PutString("ok3 ");

    return 0;
}

