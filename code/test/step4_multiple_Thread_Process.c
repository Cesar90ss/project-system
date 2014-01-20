#include "syscall.h"
#define NB_THREADS 12
#define NULL 0

void *fun(void *arg)
{
  PutString("I am a thread in a process\n");
  return 0;
}
int main()
{
  char i;
  char id[NB_THREADS];
  for(i=0;i<NB_THREADS; i++)
  {
    id[(int)i] = UserThreadCreate(&fun,NULL);
  }
  for(i=0;i<NB_THREADS; i++)
  {
    id[(int)i] = UserThreadJoin(id[(int)i],NULL);
  }
  PutString("Child ending\n");
  return 0;
}
