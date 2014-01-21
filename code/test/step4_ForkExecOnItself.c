#include "syscall.h"

int main(){

 if(ForkExec("step4_ForkExecOnItself") < 0)
 {
    PutString("Error\n");
    return -1;
 }
 else
 {
   PutString("Hello Myself\n");
 }
 return 0;
}