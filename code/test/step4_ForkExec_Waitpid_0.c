#include "syscall.h"

int main()
{
  if(Waitpid(0) == -3)
  {
    PutString("I can not wait for myself ");
  }
  return 0;
}