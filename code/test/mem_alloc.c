#include "libc.c"

/**
 * Policy should be define inside Makefile
 **/
#ifdef FIRST_FIT
    #ifdef BEST_FIT
        #error "Only on memory allocation policy should be defined"
    #else
        #ifdef WORSE_FIT
            #error "Only on memory allocation policy should be defined"
       #endif
    #endif
#else
    #ifdef BEST_FIT
        #ifdef WORSE_FIT
            #error "Only on memory allocation policy should be defined"
        #endif
    #endif
#endif


// By default define FIRST_FIT
#ifndef FIRST_FIT
    #ifndef BEST_FIT
        #ifndef WORSE_FIT
            #define FIRST_FIT 1
        #endif
    #endif
#endif

#ifndef MEMORY_SIZE
#define MEMORY_SIZE 2048 // Should be a multiple of PageSize
#endif

/* Structure declaration for a free block */
typedef struct free_block
{
    int size;
    struct free_block *next;
} free_block_s, *free_block_t;

/* Structure declaration for an occupied block */
typedef struct
{
    int size;
} busy_block_s, *busy_block_t;


/* Pointer to the first free block in the memory */
free_block_t first_free;

/* Pointer to begin & end of heap */
int heap_start;
int heap_top;

/* Semaphore for concurrency */
int big_lock;
int init_flag = 0;

#define ULONG(x)((long unsigned int)(x))
#define max(x,y) (x>y?x:y)

void memory_init(void)
{
    /**
     * Heaps init, ask for all pages now
     **/
    heap_start = AllocPageHeap();
    if (heap_start == -1)
    {
        PutString("Error while requesting page for Heap\n");
        Exit(0);
    }

    heap_top = heap_start;

    int i;
    for (i = 0; heap_start + MEMORY_SIZE >= heap_top; i++)
    {
        heap_top = AllocPageHeap();
        if (heap_top == -1)
        {
            PutString("Error while requesting page for Heap\n");
            Exit(0);
        }
    }

    /**
     * Initialize the memory with a first free block
     **/
    first_free = (void*)heap_start;
    first_free->size = MEMORY_SIZE;
    first_free->next = NULL;

    /**
     * One semaphore, one big lock
     **/
    big_lock = UserSemaphoreCreate("Malloc sem", 1);
    if (big_lock == -1)
    {
        PutString("Error while initializing semaphore\n");
        Exit(0);
    }

    init_flag = 1;
}


int not_all_freed(void)
{
    return (char*)heap_start != (char*)first_free ||
        (void*)first_free->next != NULL;
}

int check_free_block(free_block_t block)
{
    // Size is logical
    if (block->size < 0 || block->size > MEMORY_SIZE)
        return 0;

    // Pointer + size does not goes out
    if ((char*)block + block->size > (char *)heap_start + MEMORY_SIZE)
        return 0;

    // Next is inside memory
    if (block->next != NULL && ((char*)block->next < (char*)first_free || (char*)block->next >= (char *)heap_start + MEMORY_SIZE - sizeof(free_block_s)))
        return 0;

    return 1;
}

char *memory_alloc(int size)
{
    // Monitor pattern
    int ret;

    if ((ret = UserSemaphoreP(big_lock)) != 0)
    {
        PutString("Error while taking malloc semaphore ");
        PutInt(big_lock);
        PutInt(ret);
        PutChar('\n');
        Exit(0);
    }

    /**
     * Go trough free blocks and the first fitted one
     **/
    free_block_t current = first_free;
    free_block_t previous = first_free;

    free_block_t currentElected = NULL;
    free_block_t previousElected = NULL;

    unsigned int total_size_of_free_block = 0;
    unsigned int total_size_of_busy_block = 0;
    void* addr_of_next_free_block = NULL;

#ifndef FIRST_FIT
    unsigned long int minMaxSize = 0;
#endif

    // Minimum space for a free structure
    if (size < sizeof(free_block_s) - sizeof(busy_block_s))
        size = sizeof(free_block_s) - sizeof(busy_block_s);

    total_size_of_busy_block = (unsigned int)size + sizeof(busy_block_s);

    // Must be 4 bytes aligned
    if (total_size_of_busy_block % 4 != 0)
        total_size_of_busy_block += 4 - (int)total_size_of_busy_block % 4;

    while(current != NULL)
    {
        // Check metadata corruption
        if (!check_free_block(current))
        {
            PutString("ERROR : metadata corruption detected.\n");
            Exit(0);
        }

        total_size_of_free_block = current->size;

#ifdef FIRST_FIT
        // Need size of the allocated block
        if (total_size_of_free_block >= total_size_of_busy_block)
        {
            currentElected = current;
            previousElected = previous;
            break;
        }
#else
#ifdef BEST_FIT
        // Need size of the allocated block && update minimum
        if (total_size_of_free_block >= total_size_of_busy_block &&
            (minMaxSize == 0 || minMaxSize > total_size_of_free_block))
        {
            minMaxSize = total_size_of_free_block;
            currentElected = current;
            previousElected = previous;
        }
#else // WORSE_FIT
        // Need size of the allocated block && update maximum
        if (total_size_of_free_block >= total_size_of_busy_block &&
            (minMaxSize == 0 || minMaxSize < total_size_of_free_block))
        {
            minMaxSize = total_size_of_free_block;
            currentElected = current;
            previousElected = previous;
        }
#endif
#endif

        // Just go ahead
        previous = current;
        current = current->next;
    }


    // Found a suitable slot
    if (currentElected != NULL)
    {
        total_size_of_free_block = currentElected->size;

        // Create new free bloc if needed (if the size use in the block is less than the total size of the previousElected free bloc)
        if (total_size_of_free_block - total_size_of_busy_block >= sizeof(free_block_s))
        {
            addr_of_next_free_block = (void*)((char*)currentElected + total_size_of_busy_block);

            (*(free_block_t)(addr_of_next_free_block)).size = total_size_of_free_block - total_size_of_busy_block;
            (*(free_block_t)(addr_of_next_free_block)).next = currentElected->next;

            // Link previousElected to the next (if it's the first just update first_free)
            if (currentElected == first_free)
                first_free = addr_of_next_free_block;
            else
                previousElected->next = addr_of_next_free_block;
        }
        // Else just link previousElected to the next
        else
        {
            // Give more than needed space (if there is one byte left)
            total_size_of_busy_block = total_size_of_free_block;

            if (currentElected == first_free)
                first_free = currentElected->next;
            else
                previousElected->next = currentElected->next;
        }

        // Set the size of the busy block
        (*(busy_block_t)currentElected).size = total_size_of_busy_block - sizeof(busy_block_s);
    }

    // Monitor pattern
    if (UserSemaphoreV(big_lock) != 0)
    {
        PutString("Error while releasing malloc semaphore ");
        PutInt(big_lock);
        PutChar('\n');
        Exit(0);
    }

    // If no suitable block found, return NULL
    if (currentElected == NULL)
        return NULL;
    else
        return (char*)((char*)currentElected + sizeof(busy_block_s));
}

void memory_free(char *p)
{

    // Monitor pattern
    if (UserSemaphoreP(big_lock) != 0)
    {
        PutString("Error while taking free semaphore ");
        PutInt(big_lock);
        PutChar('\n');
        Exit(0);
    }

    free_block_t before = first_free;
    free_block_t after = first_free;
    busy_block_t pointer = (busy_block_t)((char*)p - sizeof(busy_block_s));

    char merge_with_before = 0;
    char merge_with_after = 0;
    char has_before = 1;

    // Fin before & after block for *pointer
    while (before != NULL)
    {
        // Check metadata corruption
        if (!check_free_block(before))
        {
            PutString("ERROR : metadata corruption detected - free.\n");
            Exit(0);
        }

        // Suppose that p is valid
        if ((char*)before + before->size <= (char *)pointer)
        {
            // Check if the next of before is after our block p
            if (before->next == NULL || (char *)before->next + before->next->size > (char*)pointer)
            {
                after = before->next;
                break;
            }
        }

        before = before->next;
    }

    // Check if it's a valid free
    char *upto = (after == NULL) ? (char *)heap_start : (char *)after;
    char *from = (p < (char*)first_free) ? (char *)heap_start : (char*)before  + before->size;
    busy_block_t cur = (busy_block_t)from;

    while ((char*)cur < upto && (char*)cur + sizeof(busy_block_s) != p)
        cur = (busy_block_t)((char *)cur + cur->size + sizeof(busy_block_s));

    if ((char *)cur >= upto)
    {
        PutString("ERROR : Ask for an invalid free address at ");
        PutInt((int)((char*)p - heap_start));
        PutString("\n");
        Exit(0);
    }

    // Update size
    pointer->size = pointer->size + sizeof(busy_block_s);

    // If no before
    if ((char*)pointer < (char*)first_free)
    {
        (*(free_block_t)pointer).next = first_free;
        first_free = (free_block_t)pointer;
        has_before = 0;
    }
    // If no after
    else if (after == NULL)
    {
        before->next =(free_block_t)pointer;
        (*(free_block_t)pointer).next = NULL;
    }
    // Between block
    else
    {
        (*(free_block_t)pointer).next = after;
        before->next = (free_block_t)pointer;
    }

    // Check merge with after
    if ((*(free_block_t)pointer).next != NULL &&
        (char*)pointer + (*(free_block_t)pointer).size == (char*)(*(free_block_t)pointer).next)
        merge_with_after = 1;

    // Check merge with before
    if (has_before && (char*)before + before->size == (char*)before->next)
        merge_with_before = 1;

    // Do merging
    if (merge_with_before == 1 && merge_with_after == 0)
    {
        free_block_t tmp = (free_block_t)pointer;

        before->next = tmp->next;
        before->size = before->size + tmp->size;
    }
    else if (merge_with_before == 0 && merge_with_after == 1)
    {
        free_block_t tmp = (free_block_t)pointer;

        tmp->next = after->next;
        tmp->size = tmp->size + after->size;
    }
    // Both sides
    else if (merge_with_before == 1 && merge_with_after == 1)
    {
        free_block_t tmp = (free_block_t)pointer;

        before->next = after->next;
        before->size = before->size + tmp->size + after->size;
    }

    // Monitor pattern
    if (UserSemaphoreV(big_lock) != 0)
    {
        PutString("Error while releasing free semaphore ");
        PutInt(big_lock);
        PutChar('\n');
        Exit(0);
    }
}

void *malloc(size_t size)
{
    if(!init_flag)
        memory_init();

    return (void*)memory_alloc((size_t)size);
}

void free(void *p)
{
    if (p == NULL) return;
    memory_free((char*)p);
}

void *realloc(void *ptr, size_t size)
{
    if(ptr == NULL)
        return memory_alloc(size);

    busy_block_t bb = ((busy_block_t)ptr) - 1;
    PutString("Reallocating ");
    PutInt(bb->size);
    PutString("bytes to ");
    PutInt((int)size);
    PutChar('\n');

    if(size <= bb->size)
        return ptr;

    char *new = memory_alloc(size);
    memcpy(new, (void*)(bb+1), bb->size);
    memory_free((char*)(ptr));
    return (void*)(new);
}

void *calloc(size_t nmemb, size_t size)
{
    char *new = memory_alloc(size * nmemb);
    memset(new, 0, nmemb * size);
    return (void*)(new);
}
