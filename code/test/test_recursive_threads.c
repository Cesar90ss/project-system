#include "syscall.h"


#define NULL 0



void *thread1_1(void *arg)
{
  PutString("We are in the child thread");
  PutChar(' ');
  return NULL;
}

void * thread1(void *arg)
{
  UserThreadJoin(UserThreadCreate(&thread1_1, NULL),NULL);
  PutString("We are in the first thread");
  PutChar(' ');
  return NULL;
}

int main()
{
  UserThreadJoin(UserThreadCreate(&thread1, NULL),NULL);
  PutString("We are in the main thread");
  PutChar(' ');
  return 0;
}