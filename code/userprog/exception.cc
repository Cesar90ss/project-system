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
extern SynchConsole *synchconsole;
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
    char* c = new char[MAX_STRING_SIZE + 1];
    int really_write = copyStringFromMachine(from,c,MAX_STRING_SIZE);
    c[really_write] = '\0';
    DEBUG('a', "Putstring %s\n", c);
    synchconsole->SynchPutString(c);
    delete [] c;
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
	char* c = new char[MAX_STRING_SIZE + 1];
	int write = copyStringFromMachine(from,c,MAX_STRING_SIZE);
	c[write] = '\0';
	t->ForkExec(c);
	// TODO return the pid not just 0 
	machine->WriteRegister(2,0);
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
			break;
		}
		case ReadOnlyException:
		{
			// Write attempted to page marked "read-only"
			printf("Read Only exception %d %d\n", which, type);
			break;
		}
		case BusErrorException:
		{
			// Translation resulted in an invalid physical address
			printf("Bus error exception %d %d\n", which, type);
			break;
		}
		case AddressErrorException:
		{
			// Unaligned reference or one that was beyond the end of the address space
			printf("Address error exception %d %d\n", which, type);
			break;
		}
		case OverflowException:
		{
			// Integer overflow in add or sub.
			printf("Overflow exception %d %d\n", which, type);
			break;
		}
		case IllegalInstrException:
		{
			// Unimplemented or reserved instr.
			printf("Illegal instruction exception %d %d\n", which, type);
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
