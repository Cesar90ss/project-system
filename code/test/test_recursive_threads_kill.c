#include "syscall.h"


#define NULL 0



void *thread1_1(void *arg)
{
  int i;
  for(i=0; i< 50; i++); //so that "parent" dies first
  PutString("We are in the child thread");
  PutChar(' ');
  return NULL;
}

void * thread1(void *arg)
{
  UserThreadCreate(&thread1_1, NULL);
  UserThreadExit(0);
  return NULL;
}

int main()
{
  UserThreadJoin(UserThreadCreate(&thread1, NULL),NULL);
  PutString("We are in the main thread");
  PutChar(' ');
  return 0;
}