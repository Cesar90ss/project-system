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
extern ProcessMgr *processMgr;
extern PostOffice *postOffice;
#include "../network/post.h"
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
    int semaphore_id = currentThread->space->SemaphoreCreate(name,value);

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
	{
        delete t;
	}

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
      {
          machine->WriteMem(retval, 4, retProc);
      }
  }

}
//----------------------//
void switch_CheckEnd()
{
	int pid = machine->ReadRegister(4);
	machine->WriteRegister(2, processMgr->ProcessEnded(pid));
}
//----------------------//
void switch_Listen()
{
	#ifdef NETWORK
	int local_port = machine->ReadRegister(4);

	//check if the port exists
	if(local_port<0 && local_port>=NB_BOX)
	{
		machine->WriteRegister(2,-1);
		return;
	}
	
	//if the corresponding mailbox is already listenning
	if(postOffice->IsListening(local_port))
	{
		machine->WriteRegister(2,-2);
		return;
	}

	//else create a socket in listenning_mode for this box in the addrSpace list of socket.
	NachosSocket* socket;
	int sid = currentThread->space->SocketCreate( &socket, SOCKET_LISTENING, 0, 0,(int) local_port);
	
	//put the mailbox in listenning mode
	postOffice->EnableListening(local_port,socket);
	
	//the listenning socket can only be use for accept or close. (else error)
	//return the listening socket id
	machine->WriteRegister(2, sid);
	#else
	synchconsole->SynchPutString("Network disabled, cannot execute Listen syscall\n");
	ASSERT(FALSE);
	#endif //NETWORK
}
//----------------------//
void switch_Accept()
{	
	int listener_sid = machine->ReadRegister(4);
	
	
	#ifdef NETWORK

	NachosSocket* listener =(NachosSocket*) currentThread->space->GetSocketPointer(listener_sid);
	if(listener == NULL)
	{
		//this socket does not exist
		machine->WriteRegister(2,-1);
		return;
	}
	
	if(!listener->IsListening())
	{
		//try to accept on a socket wich is not listening
		machine->WriteRegister(2,-2);
		return;
	}
	
	//wait on the listening list of the mailbox
	Mail *request = listener->PickARequest();
	
	//extract this from the message
	int machine_from = request->pktHdr.from;
	int port_from = request->mailHdr.from;
	
	//verify is this socket does not already exist (same machine with same port try to connect in this local port)
	int error;
	NachosSocket **socket_slot = NULL;
	if((error = postOffice->ReserveSlot(socket_slot, listener->LocalPort(), machine_from, port_from)) < 0)
	{
		//error = -1 is no free slot, -2 if this connection already exist
		machine->WriteRegister(2,error-2);
		return;
	}
	
	//create a connected socket with information in the received message, take a place in the mailbox list of socket
	/*int socket_sid =*/(void ) currentThread->space->SocketCreate
		(socket_slot, SOCKET_CONNECTED, machine_from, port_from, listener->LocalPort());

	//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	/*int loop;
	do
	{
		//send a confirmation message
		(*socket_slot)->SendConfirmation();
		// And wait for handshake

	 
	} while ( (*socket_slot)->Receive() == 0 )*/

	
	#else
	synchconsole->SynchPutString("Network disabled, cannot execute Accept syscall\n");
	ASSERT(FALSE);
	#endif //NETWORK
}
//----------------------//
void switch_Connect()
{
	#ifdef NETWORK
	int remote_machine = machine->ReadRegister(4);
	int remote_port = machine->ReadRegister(5);
	int error;
	int random_port = 0; /*SHOULD BE RANDOM*/

	//create a socket in a mailbox (search for a local free port somewhere)
	//we need to dispatch connection on different mailbox (if all connections in one, it coulb be slow)
	NachosSocket **socket_slot = NULL;
	while((error = postOffice->ReserveSlot(socket_slot, random_port, remote_machine, remote_port)) < 0);
	
	//send a connection request to the remote_machine/remote_port with the port we define above
	//TODO...
	int socket_sid = currentThread->space->SocketCreate
		(socket_slot, SOCKET_CONNECTED, remote_machine, remote_port, random_port);
	
	//send a request message (retry some times if no response and then close the socket if no response at all)
	(*socket_slot)->SendRequest(); //envoi et n'attend rien

	(*socket_slot)->Receive(NULL,0);
	//check if we receive from the good person and check if its a connection confirmation
	
	machine->WriteRegister(2,socket_sid);
		
	//synchconsole->SynchPutString("Unimplemented Connect\n");
	#else
	synchconsole->SynchPutString("Network disabled, cannot execute Connect syscall\n");
	ASSERT(FALSE);
	#endif //NETWORK
}
//----------------------//
void switch_Send()
{
	#ifdef NETWORK
	int socket = machine->ReadRegister(4);
	int buffer = machine->ReadRegister(5);
	int size = machine->ReadRegister(6);
    char c[MAX_STRING_SIZE + 1];
    int really_write = copyStringFromMachine(buffer,c,size);
    c[really_write] = '\0';
	
	NachosSocket *socket_pointer;
	socket_pointer = currentThread->space->GetSocketPointer(socket);

    socket_pointer->Send(c, really_write);
	//NachosSocket *new_connection_place = currentThread->space->GetSocketPointer(socket);
	
	//synchconsole->SynchPutString("Unimplemented Send\n");
	#else
	synchconsole->SynchPutString("Network disabled, cannot execute Send syscall\n");
	ASSERT(FALSE);
	#endif //NETWORK
}
//----------------------//
void switch_Receive()
{
	#ifdef NETWORK
	int socket = machine->ReadRegister(4);
	char buffer[MAX_STRING_SIZE];
	int size = machine->ReadRegister(6);
	( (NachosSocket*) currentThread->space->GetSocketPointer(socket) )->Receive(buffer, size);
	copyStringToMachine(machine->ReadRegister(5),buffer,size);
	//synchconsole->SynchPutString("Unimplemented Receive\n");
	#else
	synchconsole->SynchPutString("Network disabled, cannot execute Receive syscall\n");
	ASSERT(FALSE);
	#endif //NETWORK
}
//----------------------//
void switch_Disconnect()
{
	#ifdef NETWORK
	//synchconsole->SynchPutString("Unimplemented Disconnect\n");
	int socket = machine->ReadRegister(4);
	( (NachosSocket*) currentThread->space->GetSocketPointer(socket))->Disconnect();
	#else
	synchconsole->SynchPutString("Network disabled, cannot execute Disconnect syscall\n");
	ASSERT(FALSE);
	#endif //NETWORK
}
//----------------------//
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
				case SC_Listen:
                {
                    switch_Listen();
                    break;
                }
                case SC_Accept:
                {
                    switch_Accept();
                    break;
                }
                 case SC_Connect:
                {
                    switch_Connect();
                    break;
                }
                 case SC_Send:
                {
                    switch_Send();
                    break;
                }
                 case SC_Receive:
                {
                    switch_Receive();
                    break;
                }
                 case SC_Disconnect:
                {
                    switch_Disconnect();
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
