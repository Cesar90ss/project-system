#include "syscall.h"
#define NB_LOOP 10

int main()
{
  unsigned int store[NB_LOOP];
  int i,j;
  for(i=0;i<NB_LOOP;i++)
  {
    store[i]=ForkExec("run2");
  }
  for(j=0;j<NB_LOOP;j++)
  {
      Waitpid(store[j], 0);
  }
  PutString("in my world the parent should come after the child ");
  return 0;
}
