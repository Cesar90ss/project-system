// userthread.cc
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "synchconsole.h"
#include "addrspace.h"
#include "synch.h"
#include "userthread.h"


//----------------------------------------------------------------------
// StartThread
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
typedef struct
{
  int func;
  int arg;
}userfunc;

void StartUserThread(int f)
{
    userfunc *uf = (userfunc*)f;
    machine->WriteRegister(PCReg,uf->func); //put the function to the PC register
    machine->WriteRegister(NextPCReg,uf->func+sizeof(int));
    machine->WriteRegister(4,uf->arg);	//put the arg to register 4
    printf("value of function : %d",uf->func); // TODO :To be removed, just to see if function address is coherent
    machine->Run();
    return;
}

int do_UserThreadCreate(int f, int arg)
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff); 		//Block Interrupt to be atomic
    Thread* t = new  Thread("NewThread");
    userfunc *uf = new userfunc;				//define the struct
    
    uf->func=f;							//pack the function
    uf->arg=arg;    						//pack the argument
    
    int stack = t->space->GetNewUserStack();
    if(stack == -1)
      return -1;
    
    t->userRegisters[StackReg]=stack;
    t->userStack=stack;
    
    t->Fork(StartUserThread,(int)uf);
        
    (void) interrupt->SetLevel (oldLevel);
    return t->GetTid();
}

void do_UserThreadExit()
{
    currentThread->Finish();
}