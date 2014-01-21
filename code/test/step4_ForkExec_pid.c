#include "syscall.h"

int main()
{
  PutInt(ForkExec("step4_ForkExec_helloworld"));
  PutChar('\n');
  PutInt(ForkExec("step4_ForkExec_helloworld"));
  PutChar('\n');
  return 0;
}