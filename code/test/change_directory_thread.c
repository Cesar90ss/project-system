#include "syscall.h"
#define NULL 0

int sem;

void *fn(void *arg)
{
    UserSemaphoreP(sem);

    int fd = Open("test");
    char c;

    Read(fd, &c, 1);
    PutChar(c);
    Close(fd);

    return NULL;
}

int main()
{
    char c;
    char cur_dir[100];
    sem = UserSemaphoreCreate("", 0);
    int tid = UserThreadCreate(&fn, 0);

    PutString(GetCurrentDirectory(cur_dir));
    SetCurrentDirectory("a");
    PutString(GetCurrentDirectory(cur_dir));

    int fd = Open("test");
    Read(fd, &c, 1);
    PutChar(c);
    Close(fd);

    UserSemaphoreV(sem);

    UserThreadJoin(tid, NULL);

    return 0;
}
