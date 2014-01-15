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
#include "thread.h"


//----------------------------------------------------------------------
// StartThread
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
typedef struct
{
    int funcWrapper;
    int funcUser;
    int arg;
}userfunc;

void StartUserThread(int f)
{
    userfunc *uf = (userfunc*)f;
    machine->WriteRegister(PCReg,uf->funcWrapper); //put the function to the PC register
    machine->WriteRegister(NextPCReg,uf->funcWrapper+sizeof(int));

    machine->WriteRegister(4, uf->funcUser);	//put the user function to register 4
    machine->WriteRegister(5, uf->arg);	        //put the arg to register 5
    DEBUG('a', "value of stack : %d\n",machine->ReadRegister(StackReg));
    int * ptr = (int*) &machine->mainMemory[machine->ReadRegister(StackReg)];
    DEBUG('a', "Value at stack bottom : %d\n", *ptr);
    machine->Run();
    return;
}

int do_UserThreadCreate(int fnWrapper, int fnUser, int arg)
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff);      //Block Interrupt to be atomic
    Thread* t = new  Thread("NewThread");
    userfunc *uf = new userfunc;				//define the struct

    uf->funcWrapper = fnWrapper;                                 //pack the functionWrapper
    uf->funcUser = fnUser;
    uf->arg = arg;                                //pack the argument

    // Space will be attached to the new thread insinde Thread::Fork
    // t->space = currentThread -> space;

    int stack = currentThread->space->GetNewUserStack();
    if(stack == -1)
        return -1;

    t->userRegisters[StackReg]=stack;
    t->userStack=stack;
    DEBUG('a', "value of stack : %d\n", stack);

    t->Fork(StartUserThread, (int)uf);

    (void) interrupt->SetLevel (oldLevel);
    return t->GetTid();
}

void do_UserThreadExit()
{
    currentThread->SetUserReturn(machine->ReadRegister(4));
    currentThread->Finish();
}

int do_UserThreadJoin()
{
    // Get thread id
    int tid = machine->ReadRegister(4);

    // Get return @ for exit code
    int retval = machine->ReadRegister(5);

    // Find thread with this id
    Thread* target = currentThread->space->GetThreadById(tid);
    if (target == NULL)
        return -1;

    // Semaphore are interruptible, can join here
    target->Join(currentThread);

    // Fill retval with return value if not null
    DEBUG('t', "Thread exit with return %d for address %d\n", currentThread->GetUserReturn(), retval);
    if (retval != 0)
        machine->WriteMem(retval, 4, currentThread->GetUserReturn());

    return 0;
}
