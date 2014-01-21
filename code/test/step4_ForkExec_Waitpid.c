#include "syscall.h"

int main()
{
  Waitpid(ForkExec("step4_LoopForWaitpid"));
  PutString("in my world the parent should come after the child...\n");
  return 0;
}