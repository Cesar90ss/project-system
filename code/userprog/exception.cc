// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "synchconsole.h"
#ifdef USER_PROGRAM
#include "userthread.h"
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}
//----------------------------------------------------------------------
// Here are all the syscall functions we call with the sycall type switch
//----------------------------------------------------------------------
void switch_Halt()
{
    DEBUG ('m', "Shutdown, initiated by user program.\n");

    AddrSpace::Exit(true); // Force halt
}
//----------------------//
void switch_Exit()
{
    processMgr->SetReturn(currentThread->space->GetPid(), machine->ReadRegister(4));

    AddrSpace::Exit();
}
//----------------------//
#ifdef USER_PROGRAM
void switch_Putchar()
{
    char c = (char)machine->ReadRegister(4);//order of the bit endian
    DEBUG('a', "Putchar \n", machine->ReadRegister(4));
    synchconsole->SynchPutChar(c);
}
//----------------------//
void switch_Getchar()
{
    int c = synchconsole->SynchGetChar();//change to int to make it work to EOF
    machine->WriteRegister(2,c);
    DEBUG('a', "Getchar %c\n",c);
}
//----------------------//
void switch_Putstring()
{
    int from = machine->ReadRegister(4);
    char c[MAX_STRING_SIZE + 1];
    int really_write = copyStringFromMachine(from,c,MAX_STRING_SIZE);
    c[really_write] = '\0';
    DEBUG('a', "Putstring %s\n", c);
    synchconsole->SynchPutString(c);
}
//----------------------//
void switch_Getstring()
{
    int n = (int)machine->ReadRegister(5);
    char* buffer = new char[n];

    if (synchconsole->SynchGetString(buffer, n) == NULL)
    {
        machine->WriteRegister(2, (int)NULL);
    }
    else
    {
        //copy buffer to string
        copyStringToMachine(machine->ReadRegister(4), buffer, n);
        machine->WriteRegister(2, machine->ReadRegister(4));
        DEBUG('a', "GetString %s\n", buffer);
    }

    delete [] buffer;
}
//----------------------//
void switch_Putint()
{
    int num = machine->ReadRegister(4);
    synchconsole->SynchPutInt(num);
    DEBUG('a', "PutInt %d\n", num);
}
//----------------------//
void switch_Getint()
{
    int p =  machine->ReadRegister(4);
    int num;
    int error_value = 0;

    //Try to write at @p before consume input
    if(!machine->WriteMem(p, sizeof(int), (int)0))
    {
        //-2 convention for non valid adress memory
        machine->WriteRegister(2,-2);
        DEBUG('a', "GetInt : bad adress %d\n", p);
        return;
    }
    error_value = synchconsole->SynchGetInt(&num);
    machine->WriteRegister(2,error_value);
    machine->WriteMem(p, sizeof(int), num);
    DEBUG('a', "GetInt %d\n", error_value);
}
//----------------------//
void switch_UserThreadCreate()
{
    int fnWrapper = machine->ReadRegister(4);
    int fnUser = machine->ReadRegister(5);
    int arg = machine->ReadRegister(6);

    DEBUG('t', "Create user thread on function at address %i and arg at address %i\n", fnUser, arg);
    int id;
    if((id = do_UserThreadCreate(fnWrapper, fnUser, arg)) == -1)
    {
        //creation failed
        DEBUG('t', "Syscall failed to create a new user thread\n");
        machine->WriteRegister(2,-1);
    }
    else
    {
        //creation succeed
        DEBUG('t', "Thread creation succesfull with id %i\n", id);
        machine->WriteRegister(2,id);
    }
}
//----------------------//
void switch_UserThreadJoin()
{
    machine->WriteRegister(2, do_UserThreadJoin());
}
//----------------------//
void switch_UserThreadExit()
{
    do_UserThreadExit();
}
//----------------------//
void switch_UserSemaphoreCreate()
{
    int from = machine->ReadRegister(4);
    int value =  machine->ReadRegister(5);
    char* name = new char[MAX_STRING_SIZE + 1];
    int name_size = copyStringFromMachine(from,name,MAX_STRING_SIZE);
    name[name_size] = '\0';

    DEBUG('t', "Creation of semaphore %s\n", name);
    int semaphore_id = currentThread->space->CreateSemaphore(name,value);

    machine->WriteRegister(2,semaphore_id);
    delete [] name;
}
//----------------------//
void switch_UserSemaphoreP()
{
    machine->WriteRegister(2,currentThread->space->SemaphoreP(machine->ReadRegister(4)));
}
//----------------------//
void switch_UserSemaphoreV()
{
    machine->WriteRegister(2,currentThread->space->SemaphoreV(machine->ReadRegister(4)));
}
//----------------------//
void switch_UserSemaphoreDestroy()
{
    machine->WriteRegister(2,currentThread->space->SemaphoreDestroy(machine->ReadRegister(4)));
}
//----------------------//
void switch_ForkExec()
{
    Thread* t = new Thread("ThreadForkExec");
    int from = machine->ReadRegister(4);

    t->progName = new char[MAX_STRING_SIZE + 1];

    int write = copyStringFromMachine(from, t->progName, MAX_STRING_SIZE);
    t->progName[write] = '\0';

    int pid = t->ForkExec(t->progName);

    if (pid < 0)
        delete t;

    machine->WriteRegister(2,pid);
}
//----------------------//
void switch_AllocPageHeap()
{
    machine->WriteRegister(2, currentThread->space->GetHeapPage());
}
//----------------------//
void switch_FreePageHeap()
{
    machine->WriteRegister(2, currentThread->space->FreeHeapPage());
}
//----------------------//
void switch_Waitpid()
{
  unsigned int pid = machine->ReadRegister(4);

  int ret = processMgr->ProcessWaitP(pid);

  machine->WriteRegister(2, ret);

  if (ret == 0)
  {
      // Get return @ for exit code
      int retval = machine->ReadRegister(5);

      // Fill user return
      int retProc = processMgr->GetReturn(pid);

      // Fill retval with return value if not null
      if (retval != 0)
          machine->WriteMem(retval, 4, retProc);
  }

}
//----------------------//
void switch_CheckEnd()
{
  int pid = machine->ReadRegister(4);
  machine->WriteRegister(2, processMgr->ProcessEnded(pid));
}

void switch_Open()
{
    // Get file name from user space
    char *filename = new char[MAX_STRING_SIZE + 1];
    int write = copyStringFromMachine(machine->ReadRegister(4), filename, MAX_STRING_SIZE);
    filename[write] = '\0';

    // Expand filename
    char *absname = fileSystem->ExpandFileName(filename);
    delete filename;

    // Try to open file
    int ret = currentThread->space->FileOpen(absname);
    delete absname;

    // Notify user of result
    machine->WriteRegister(2, ret);
}

void switch_Close()
{
    // Read id from userspace
    int id = machine->ReadRegister(4);

    // Close file & notify user
    machine->WriteRegister(2, currentThread->space->FileClose(id));
}

void switch_Read()
{
    // Retrieve arguments
    int id = machine->ReadRegister(4);
    int to = machine->ReadRegister(5);
    int size = machine->ReadRegister(6);

    // Read from file & notify user
    machine->WriteRegister(2, currentThread->space->FileRead(id, to, size));
}

void switch_Write()
{
    // Retrieve arguments
    int id = machine->ReadRegister(4);
    int from = machine->ReadRegister(5);
    int size = machine->ReadRegister(6);

    // Write from file & notify user
    machine->WriteRegister(2, currentThread->space->FileWrite(id, from, size));
}

void switch_Seek()
{
    // Retrieve arguments
    int id = machine->ReadRegister(4);
    int position = machine->ReadRegister(5);

    // Seek to position
    machine->WriteRegister(2, currentThread->space->FileSeek(id, position));
}

void switch_GetCurrentDirectory()
{
    int to = machine->ReadRegister(4);
    int n = strlen(currentThread->GetCurrentDirectory());

    // Copy buffer to string
    copyStringToMachine(to, currentThread->GetCurrentDirectory(), n);
    machine->WriteRegister(2, machine->ReadRegister(4));
}

void switch_SetCurrentDirectory()
{
    int from = machine->ReadRegister(4);
    char *buffer = new char[MAX_STRING_SIZE + 1];

    // Copy buffer to string
    copyStringFromMachine(from, buffer, MAX_STRING_SIZE);
    machine->WriteRegister(2, currentThread->SetCurrentDirectory(buffer));
}

#endif //USER_PROGRAM
//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    switch (which)
    {
        /**
         * Handling syscall exceptions
         **/
        case SyscallException:
        {
            switch (type)
            {
                case SC_Halt:
                {
                    switch_Halt();
                    break;
                }
                case SC_Exit:
                {
                    switch_Exit();
                    break;
                }
                #ifdef USER_PROGRAM
                case SC_PutChar:
                {
                    switch_Putchar();
                    break;
                }
                case SC_GetChar:
                {
                    switch_Getchar();
                    break;
                }
                case SC_PutString:
                {
                    switch_Putstring();
                    break;
                }
                case SC_GetString:
                {
                    switch_Getstring();
                    break;
                }
                case SC_PutInt:
                {
                    switch_Putint();
                    break;
                }
                case SC_GetInt:
                {
                    switch_Getint();
                    break;
                }
                case SC_UserThreadCreate:
                {
                    switch_UserThreadCreate();
                    break;
                }
                case SC_UserThreadJoin:
                {
                    switch_UserThreadJoin();
                    break;
                }
                case SC_UserThreadExit:
                {
                    switch_UserThreadExit();
                    break;
                }
                case SC_UserSemaphoreCreate:
                {
                    switch_UserSemaphoreCreate();
                    break;
                }
                case SC_UserSemaphoreP:
                {
                    switch_UserSemaphoreP();
                    break;
                }
                case SC_UserSemaphoreV:
                {
                    switch_UserSemaphoreV();
                    break;
                }
                case SC_UserSemaphoreDestroy:
                {
                    switch_UserSemaphoreDestroy();
                    break;
                }
                case SC_ForkExec:
                {
                    switch_ForkExec();
                    break;
                }
                case SC_AllocPageHeap:
                {
                    switch_AllocPageHeap();
                    break;
                }
                case SC_FreePageHeap:
                {
                    switch_FreePageHeap();
                    break;
                }
                case SC_Waitpid:
                {
                    switch_Waitpid();
                    break;
                }
                case SC_CheckEnd:
                {
                    switch_CheckEnd();
                    break;
                }
                case SC_Open:
                {
                    switch_Open();
                    break;
                }
                case SC_Close:
                {
                    switch_Close();
                    break;
                }
                case SC_Seek:
                {
                    switch_Seek();
                    break;
                }
                case SC_Read:
                {
                    switch_Read();
                    break;
                }
                case SC_Write:
                {
                    switch_Write();
                    break;
                }
                case SC_GetCurrentDirectory:
                {
                    switch_GetCurrentDirectory();
                    break;
                }
                case SC_SetCurrentDirectory:
                {
                    switch_SetCurrentDirectory();
                    break;
                }
                #endif
                default:
                {
                    printf ("Unexpected syscall type %d\n", type);
                    ASSERT (FALSE);
                }
            }
            break;
        }
        case PageFaultException:
        {
            // No valid translation found
            printf("Page fault exception %d %d\n", which, type);
            fflush(stdout);
            AddrSpace::Exit();
            break;
        }
        case ReadOnlyException:
        {
            // Write attempted to page marked "read-only"
            printf("Read Only exception %d %d\n", which, type);
            fflush(stdout);
            AddrSpace::Exit();
            break;
        }
        case BusErrorException:
        {
            // Translation resulted in an invalid physical address
            printf("Bus error exception %d %d\n", which, type);
            fflush(stdout);
            AddrSpace::Exit();
            break;
        }
        case AddressErrorException:
        {
            // Unaligned reference or one that was beyond the end of the address space
            printf("Address error exception %d %d\n", which, type);
            fflush(stdout);
            AddrSpace::Exit();
            break;
        }
        case OverflowException:
        {
            // Integer overflow in add or sub.
            printf("Overflow exception %d %d\n", which, type);
            fflush(stdout);
            AddrSpace::Exit();
            break;
        }
        case IllegalInstrException:
        {
            // Unimplemented or reserved instr.
            printf("Illegal instruction exception %d %d\n", which, type);
            fflush(stdout);
            AddrSpace::Exit();
            break;
        }
        default:
        {
            printf ("Unexpected user mode exception %d %d\n", which, type);
            ASSERT (FALSE);
        }
    }

    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}
