#include "mem_alloc.c"

#define THREAD_NUMBER 20

void *fn(void *arg)
{
    void *m1;
    void *m2;
    void *m3;

    m1 = malloc(1);
    m2 = malloc(1);
    free(m1);
    free(m2);
    m3 = malloc(2);
    free(m3);

    return NULL;
}

int main()
{
    int id[THREAD_NUMBER];
    int i;

    // Call init explicitly to avoid semaphore creation problem in cast of
    // concurrency
    memory_init();

    for (i = 0; i < THREAD_NUMBER; i++)
    {
        id[i] = UserThreadCreate(&fn, NULL);
        if (id[i] < 0)
        {
            PutString("Thread creation failed !\n");
            return -1;
        }
    }
    for (i = 0; i < THREAD_NUMBER; i++)
    {
        if (UserThreadJoin(id[i], NULL) != 0)
        {
            PutString("Error while joining\n");
            return -1;
        }
    }

    if (!not_all_freed())
        PutString("ok");

    return 0;
}
