#include "syscall.h"
#define NB_LOOP 1000
int main()
{
  int i;
  for(i=0; i< NB_LOOP;i++); //we give the parent an occasion to write its message before
  PutString("I am the child, ");
  return 0;
}