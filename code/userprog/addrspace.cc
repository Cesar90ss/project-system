// addrspace.cc
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
        WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------


extern ProcessMgr *processMgr;

AddrSpace::AddrSpace() : max_tid(0), num_threads(0)
{
    // Init Page Table Translation
    InitTranslation();

    pid = processMgr->CreateProcess(this);

    semaphore_list = NULL;
    semaphore_counter = 0;

    // Set default current directory
    currentDirectory = new char[2];
    currentDirectory[0] = '/';
    currentDirectory[1] = '\0';

    // Init open file table as unused
    int i;
    for (i = 0; i < MAX_OPEN_FILES; i++)
        filetable[i].inUse = false;
}

void AddrSpace::LoadExecutable(OpenFile * executable)
{
    NoffHeader noffH;

    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
        (WordToHost (noffH.noffMagic) == NOFFMAGIC))
        SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

    // Compute size for code + data & allocate page for it
    unsigned int codePages = noffH.code.size + noffH.initData.size + noffH.uninitData.size;
    codePages = divRoundUp(codePages, PageSize);

    // Allocate page for code & data (suppose data just after code)
    AllocatePages(noffH.code.virtualAddr, codePages);

    // Then, copy in the code and data segments into virtual memory
    if (noffH.code.size > 0)
    {
        DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
               noffH.code.virtualAddr, noffH.code.size);

        executable->ReadAtVirtual(noffH.code.virtualAddr,
                                  noffH.code.size,
                                  noffH.code.inFileAddr);

    }
    if (noffH.initData.size > 0)
    {
        DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
               noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAtVirtual(noffH.initData.virtualAddr,
                                    noffH.initData.size,
                                    noffH.initData.inFileAddr);
    }

    // Mark page only code as readOnly
    SetPageRights(noffH.code.virtualAddr, noffH.code.size/PageSize, READONLY);

    // Init stack mgr
    stackMgr = new StackMgr(this, codePages * PageSize);
    heapMgr = new HeapMgr(this, codePages * PageSize);
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
    /**
     * Delete last semaphore
     **/
    std::map<unsigned int, ThreadInfo> copy(threads);
    std::map<unsigned int, ThreadInfo>::iterator it;
    struct ThreadInfo t;

    for (it = copy.begin(); it != copy.end(); ++it)
    {
        t = it->second;

        if (t.join != NULL)
            delete t.join;
    }

    threads.clear();

    // Delete current directory
    delete [] currentDirectory;

    // Free stack mgr
    delete stackMgr;

    // Free heap mgr
    delete heapMgr;

    // Clean page table
    CleanPageTable();

    // Clean semaphore
    CleanSemaphores();

    // Clean open files
    CleanOpenFiles();
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

    // Ask stackmgr for a stack (should be the first one)
    int stackAddr = GetNewUserStack();
    currentThread->userStack = stackAddr;

    machine->WriteRegister(StackReg, stackAddr);
    DEBUG ('a', "Initializing stack register to %d\n",
           stackAddr);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------
void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------
void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
//------------------------------------------------------------//
// Wrapper around StackMgr
int AddrSpace::GetNewUserStack()
{
#ifdef USER_PROGRAM
    return stackMgr->GetNewStack();
#else
    return 0;
#endif
}
//------------------------------------------------------------//
/**
 * Create a new semaphore named name and return the id of the semaphore
 **/
int AddrSpace::SemaphoreCreate(char *name, int val)
{
    //create a new semaphore
    id_list new_sem = new struct id_l;
    new_sem->id = semaphore_counter;
    new_sem->item = (void*)new Semaphore(name,val);

    //add it to the list
    new_sem->next = semaphore_list;
    semaphore_list = new_sem;

    //increment the id counter and return the id of the new semaphore
    semaphore_counter++;
    return new_sem->id;
}
//------------------------------------------------------------//
/**
 * Return -1 if this semaphore does not exist
 **/
int AddrSpace::SemaphoreP(int id)
{
    id_list cursor = semaphore_list;
    while(cursor!=NULL && cursor->id!=id)
    {
        cursor = cursor->next;
    }

    if(cursor == NULL)
    {
        //the semaphore doesn't exist
        return -1;
    }

    ((Semaphore*)(cursor->item))->P();
    return 0;
}
//------------------------------------------------------------//
/**
 * Return -1 if this semaphore does not exist
 **/
int AddrSpace::SemaphoreV(int id)
{
    id_list cursor = semaphore_list;
    while(cursor!=NULL && cursor->id!=id)
    {
        cursor = cursor->next;
    }

    if(cursor == NULL)
    {
        //the semaphore doesn't exist
        return -1;
    }

    ((Semaphore*)(cursor->item))->V();
    return 0;
}
//------------------------------------------------------------//
/**
 * Remove the corresponding semaphore from the list or return -1 if it doesn't exist
 **/
int AddrSpace::SemaphoreDestroy(int id)
{
    id_list current = semaphore_list;
    id_list previous = NULL;

	//search a socket with this id
    while(current!=NULL && current->id!=id)
    {
        previous = current;
        current = current->next;
    }

    if(current == NULL) //the semaphore doesn't exist
    {
        return -1;
    }

    if(previous == NULL) //the semaphore is the first of the list
    {
        semaphore_list = current->next;
        delete (Semaphore*)current->item;
        delete current;
    }
    else
    {
        previous->next = current->next;
        delete (Semaphore*)current->item;
        delete current;
    }

    return 0;
}
//------------------------------------------------------------//
/**
 * Delete all the semaphores in the list
 **/
void AddrSpace::CleanSemaphores()
{
    id_list cursor = semaphore_list;
    id_list destructor = NULL;

    while(cursor!=NULL)
    {
        destructor = cursor;
        cursor = cursor->next;
        delete (Semaphore*)destructor->item;
        delete destructor;
    }
}

//------------------------------------------------------------//
/**
 * SOCKETS
 **/
#ifdef NETWORK
int AddrSpace::SocketCreate(NachosSocket **socket, SocketStatusEnum status, int remote_machine, int remote_port, int mailbox)
{
	//create a new socket
	id_list new_sock = new struct id_l;
	new_sock->id = socket_counter;
	
	// create a new socket and add it to the struct we created
	*socket = new NachosSocket(status, remote_machine, remote_port, mailbox);
	new_sock->item = (void*) *socket;

	//add it to the list
	new_sock->next = socket_list;
	socket_list = new_sock;

	//increment the id counter and return the id of the new semaphore
	socket_counter++;
	return new_sock->id;
}
	
/**
 * Remove the corresponding socket from the list or return -1 if it doesn't exist
 **/
int AddrSpace::SocketDestroy(int id)
{
    id_list current = socket_list;
    id_list previous = NULL;

	//search a socket with this id
    while(current!=NULL && current->id!=id)
    {
        previous = current;
        current = current->next;
    }

    if(current == NULL) //the socket doesn't exist
    {
        return -1;
    }

    if(previous == NULL) //the socket is the first of the list
    {
        socket_list = current->next;
        delete (NachosSocket*)current->item;
        delete current;
    }
    else
    {
        previous->next = current->next;
        delete (NachosSocket*)current->item;
        delete current;
    }

    return 0;
}
/**
 * Delete all the socket in the list
 **/
void AddrSpace::CleanSockets()
{
    id_list cursor = socket_list;
    id_list destructor = NULL;

    while(cursor!=NULL)
    {
        destructor = cursor;
        cursor = cursor->next;
        //delete (NachosSocket*)destructor->item;
        delete destructor;
    }
}

/**
 * Search for the sopcket identified by the id and return a pointer to it
 **/
NachosSocket *AddrSpace::GetSocketPointer(int id)
{
	id_list cursor = socket_list;

	while(cursor != NULL)
	{
		if(cursor->id == id)	// Found the entry we search, return the socket.
		{
			return ( (NachosSocket*) cursor->item );
		}
		cursor = cursor->next;
	}

	return NULL; // we did not find anything
}

#endif //NETWORK

//------------------------------------------------------------//
// Wrapper around StackMgr
int AddrSpace::FreeUserStack(unsigned int addr)
{
#ifdef USER_PROGRAM
    return stackMgr->FreeStack(addr);
#else
    return -1;
#endif
}
//------------------------------------------------------------//
/**
 * Return all threads inside this @ space as a list
 **/
std::map<unsigned int, ThreadInfo> AddrSpace::GetThreads()
{
    return threads;
}
//------------------------------------------------------------//
/**
 * Mark all thread as finished when exit
 **/
void AddrSpace::KillAllThreads()
{
    /**
     * Each thread will call DetachThread, which modify threads
     * So make a copy
     **/
    std::map<unsigned int, ThreadInfo> copy(threads);
    std::map<unsigned int, ThreadInfo>::iterator it;
    struct ThreadInfo t;

    for (it = copy.begin(); it != copy.end(); ++it)
    {
        t = it->second;

        // Not the current as we need it for last context switch
        if (t.ptr == currentThread)
            continue;

        // Not thread marked as ended
        if (t.status == THREAD_ENDED)
            continue;

        // Keep main thread for last cleanup
        if (t.ptr == mainThread)
            continue;

        delete t.ptr;
    }
}
//------------------------------------------------------------//
/**
 * Suppose not inside threads
 **/
void AddrSpace::AttachThread(Thread *child)
{
    struct ThreadInfo ti;

    ti.status = THREAD_RUNNING;
    ti.ptr = child;
    ti.join = new Semaphore("Join semaphore thread", 0);

    // Insert with new tid
    threads[max_tid++] = ti;
    child->SetTid(max_tid - 1);
    child->space = this;

    num_threads++;
}
//------------------------------------------------------------//
/**
 * Suppose already inside threads
 **/
void AddrSpace::DetachThread(Thread *child)
{
    struct ThreadInfo ti;
    ti = threads[child->GetTid()];
    ti.status = THREAD_ENDED;
    threads[child->GetTid()] = ti;
    child->space = NULL;
}
//------------------------------------------------------------//
/**
 * Join P on thread
 **/
void AddrSpace::ThreadJoinP(unsigned int tid)
{
    if (threads.find(tid) == threads.end())
        return;

    threads[tid].join->P();
}
//------------------------------------------------------------//
/**
 * Join V on thread
 **/
void AddrSpace::ThreadJoinV(unsigned int tid)
{
    if (threads.find(tid) == threads.end())
        return;

    threads[tid].join->V();
}
//------------------------------------------------------------//
/**
 * Get thread by tid
 * Return NULL if non existence
 **/
Thread *AddrSpace::GetThreadById(unsigned int tid)
{
    if (threads.find(tid) == threads.end())
        return NULL;

    return threads[tid].ptr;
}
//------------------------------------------------------------//
/**
 * Check if a thread is ended previously
 **/
bool AddrSpace::ThreadEnded(unsigned int tid)
{
    if (threads.find(tid) == threads.end())
        return false;

    return threads[tid].status == THREAD_ENDED;
}
//------------------------------------------------------------//
/**
 * Set return value for a given thread
 **/
void AddrSpace::SetThreadReturn(unsigned int tid, int ret)
{
    struct ThreadInfo ti;

    ti = threads[tid];
    ti.ret = ret;
    threads[tid] = ti;
}
//------------------------------------------------------------//
/**
 * Get return value for a given thread
 **/
int AddrSpace::GetThreadReturn(unsigned int tid)
{
    if (threads.find(tid) == threads.end())
        return 0;

    return threads[tid].ret;
}
//------------------------------------------------------------//
/**
 * Get number of currently running threads
 **/
unsigned int AddrSpace::CurrentThreadNumber()
{
    return num_threads;
}
//------------------------------------------------------------//
/**
 * Exit process (notice force halt)
 **/
void AddrSpace::Exit(bool forceHalt)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    DEBUG('m', "Exit program, return code exit(%d)\n", machine->ReadRegister(4));
    // Get all threads inside @space && finished it
    currentThread->space->KillAllThreads();

    processMgr->ProcessWaitV(currentThread->space->GetPid()); //we release the lock on the semaphore(end of process)
    processMgr->EndProcess(currentThread->space);

    // If last thread, halt
    if ((int)processMgr->nbProcess == 0 || forceHalt)
    {
        Thread *t = currentThread;

        // Do a context switch to get back to main
        if (t != mainThread)
        {
            // Delete @space now
            spaceToBeDestroyed = currentThread->space;

            // Mark thread to destroy
            threadToBeDestroyed = t;
            currentThread = NULL;

            // Bit of hacky but need to switch right here
            mainThread->pushStack((int)&&lbl);

            // Now change thread
            SWITCH(t, mainThread);
            // /!\ value in stack are not the same as previously, does not use
            // it

        lbl:
            // Delete thread
            delete threadToBeDestroyed;

            // Also delete @space for cleanup
            if (mainThread->space != spaceToBeDestroyed && spaceToBeDestroyed != NULL)
                delete spaceToBeDestroyed;


            currentThread = mainThread;
        }

        // Should be main thread
        ASSERT(currentThread == mainThread);

        // Now delete main thread (currently running)
        AddrSpace *mainSpace = currentThread->space;

        threadToBeDestroyed = currentThread;
        currentThread = NULL;
        delete threadToBeDestroyed;

        // Delete space of main thread also
        delete mainSpace;

        interrupt->Halt();
    }
    else
    {
        // Clear stack
        AddrSpace *save = currentThread->space;

        currentThread->space->FreeUserStack(currentThread->userStack);
        currentThread->space->DetachThread(currentThread);
        delete save;

        currentThread->Finish();
    }

    interrupt->SetLevel(oldLevel);
}
//------------------------------------------------------------//
/**
 * Get current max tid
 **/
unsigned int AddrSpace::GetMaxTid()
{
    return max_tid;
}

/**
 * Initialize the page table
 **/
void AddrSpace::InitTranslation()
{
    numPages = NUM_VIRTUAL_PAGES;
    unsigned int size = numPages * PageSize;
    unsigned int i;

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
           numPages, size);

    // Set up the translation
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++)
    {
        pageTable[i].virtualPage = i;
        pageTable[i].physicalPage = -1;
        pageTable[i].valid = FALSE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;
    }

    // Set machine page table now for loading code
    RestoreState();
}

/**
 * Ask for allocating num pages starting at @ addr
 **/
void AddrSpace::AllocatePages(unsigned int addr, unsigned int num)
{
    unsigned int i = 0;
    unsigned int pageAddr;
    unsigned int index;
    unsigned int virtualIndex = addr / PageSize;

    // addr should be page aligned
    ASSERT(addr % PageSize == 0);

    for (i = 0; i < num; i++)
    {
        // Ask frame provider for page
        if (frameProvider->GetEmptyFrame(&pageAddr) != 0)
        {
            printf("No more physical memory !\n");
            ASSERT(FALSE);
        }

        // Compute index of page
        index = pageAddr / PageSize;

        // This page should not be already validated
        ASSERT(pageTable[virtualIndex + i].valid == FALSE);

        // Mark inside pagetable as valid & map it
        pageTable[virtualIndex + i].virtualPage = virtualIndex + i;
        pageTable[virtualIndex + i].physicalPage = index;
        pageTable[virtualIndex + i].valid = TRUE;

        DEBUG('a', "Allocate page %d -> %d - %u\n", virtualIndex + i, index, (virtualIndex + i) * PageSize);
    }
}

/**
 * Set rights r for num pages starting at addr
 * r = READONLY | READWRITE (default READWRITE)
 **/
void AddrSpace::SetPageRights(unsigned int addr, unsigned int num, enum PageRight r)
{
    unsigned int virtualIndex = addr / PageSize;
    unsigned int i;

    // addr should be page aligned
    ASSERT(addr % PageSize == 0);

    for (i = 0; i < num; i++)
    {
        switch (r)
        {
            case READONLY:
                pageTable[virtualIndex + i].readOnly = TRUE;
                break;
            case READWRITE:
                pageTable[virtualIndex + i].readOnly = FALSE;
                break;
            default:
                DEBUG('a', "Unknow right type %d\n", r);
                ASSERT(FALSE);
        }
    }
}

/**
 * Ask for deleting num pages starting at @ addr
 **/
void AddrSpace::FreePages(unsigned int addr, unsigned int num)
{
    unsigned int i = 0;
    unsigned int virtualIndex = addr / PageSize;

    // addr should be page aligned
    ASSERT(addr % PageSize == 0);

    for (i = 0; i < num; i++)
    {
        // This page should be valid
        ASSERT(pageTable[virtualIndex + i].valid == TRUE);

        // Ask frame provider deleting page
        ASSERT(frameProvider->ReleaseFrame(pageTable[virtualIndex + i].physicalPage * PageSize) == 0);

        DEBUG('a', "Deallocate page %d - %u\n", virtualIndex + i, (virtualIndex + i) * PageSize);

        // Mark inside pagetable as valid & map it
        pageTable[virtualIndex + i].physicalPage = 0;
        pageTable[virtualIndex + i].valid = FALSE;
    }
}

/**
 * Clean page table when freeing addr space
 **/
void AddrSpace::CleanPageTable()
{
    unsigned int i = 0;

    for (i = 0; i < NUM_VIRTUAL_PAGES; i++)
    {
        if (pageTable[i].valid == TRUE)
            FreePages(i * PageSize, 1);

        ASSERT(pageTable[i].valid == FALSE);
    }

    delete [] pageTable;
}


/**
 * Return the pid of the process attached to this addrspace
 **/
unsigned int AddrSpace::GetPid(void)
{
  return pid;
}


/**
 * Wrappers around heap mgr
 **/
int AddrSpace::GetHeapPage()
{
#ifdef USER_PROGRAM
    return heapMgr->AllocatePage();
#else
    return -1;
#endif
}

int AddrSpace::FreeHeapPage()
{
#ifdef USER_PROGRAM
    return heapMgr->FreePage();
#else
    return 0;
#endif
}

const char* AddrSpace::GetCurrentDirectory()
{
    return currentDirectory;
}

int AddrSpace::SetCurrentDirectory(const char* dirname)
{
    // Delete previous dir
    delete [] currentDirectory;

    char *tmp = new char[strlen(dirname) + 1];
    strcpy(tmp, dirname);
    currentDirectory = tmp;
    return 0;
}

/**
 * Close all open files
 **/
void AddrSpace::CleanOpenFiles()
{
    int i;

    for (i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (filetable[i].inUse)
        {
            // Close file
            delete filetable[i].handler;
            filetable[i].handler = NULL;

            // Delete filename
            delete [] filetable[i].absoluteName;
            filetable[i].absoluteName = NULL;

            // Detach owner
            filetable[i].owner = NULL;

            // Mark as unused
            filetable[i].inUse = false;
        }
    }
}


/**
 * Try to open a file
 *
 * Return -1 if file cannot be opened
 * Return -2 if MAX_OPEN_FILES is reached
 * Otherwise return id (\in [0; MAX_OPEN_FILES[)
 **/
int AddrSpace::FileOpen(const char* filename)
{
    int index = 0;

    while (index < MAX_OPEN_FILES && filetable[index].inUse)
        index++;

    // If no more empty slot
    if (index == MAX_OPEN_FILES)
        return -2;

    // Mark as inUse in case of context switch
    filetable[index].inUse = true;

    // Fill structure
    char *name = new char[strlen(filename) + 1];
    strcpy(name, filename);

    filetable[index].absoluteName = name;

    // Try to open file
    OpenFile *handler = fileSystem->Open(filename);
    if (handler == NULL)
    {
        filetable[index].inUse = false;
        delete [] name;
        return -1;
    }

    filetable[index].handler = handler;
    filetable[index].owner = currentThread;
    filetable[index].inUse = true;
    return index;
}

/**
 * Try to remove a file
 *
 * Return -1 if file cannot be found
 * Return -2 if the file is already opened
 * 0 otherwise
 **/
int AddrSpace::FileRemove(const char* filename)
{
    return fileSystem->Remove(filename);
}

/**
 * Close a file
 *
 * Return -1 if the file was not opened or id invalid
 * 0 otherwise
 **/
int AddrSpace::FileClose(int id)
{
    // ID validity
    if (id < 0 || id >= MAX_OPEN_FILES)
        return -1;

    // file open
    if (!filetable[id].inUse)
        return -1;

    // Clean structure
    delete filetable[id].handler;
    filetable[id].handler = NULL;

    delete [] filetable[id].absoluteName;
    filetable[id].absoluteName = NULL;
    filetable[id].owner = NULL;
    filetable[id].inUse = false;


    return 0;
}

/**
 * Write to a file
 *
 * into refers to virtual @ inside user space
 * Return -1 if the file was not opened or id invalid
 * num of bytes written otherwise
 **/
int AddrSpace::FileWrite(int id, int into, int numBytes)
{
    // ID validity
    if (id < 0 || id >= MAX_OPEN_FILES)
        return -1;

    // File open
    if (!filetable[id].inUse)
        return -1;

    // Pass request to handler
    return filetable[id].handler->WriteVirtual(into, numBytes);
}

/**
 * Read from a file
 *
 * buffer refers to virtual @ inside user space
 *
 * Return -1 if the file was not opened or id invalid
 * num of bytes written otherwise
 **/
int AddrSpace::FileRead(int id, int buffer, int numBytes)
{
    // ID validity
    if (id < 0 || id >= MAX_OPEN_FILES)
        return -1;

    // File open
    if (!filetable[id].inUse)
        return -1;

    // Pass request to handler
    return filetable[id].handler->ReadVirtual(buffer, numBytes);
}

/**
 * Seek inside a file
 *
 * Return -1 if the file was not opened or id invalid
 * 0 otherwise
 **/
int AddrSpace::FileSeek(int id, int position)
{
    // ID validity
    if (id < 0 || id >= MAX_OPEN_FILES)
        return -1;

    // File open
    if (!filetable[id].inUse)
        return -1;

    // Pass request to handler
    filetable[id].handler->Seek(position);

    return 0;
}
