// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "stackmgr.h"
#include "heapmgr.h"
#include "synch.h"
#include "processmgr.h"

#include <map>
#ifdef NETWORK
#include "post.h"
#endif
class Thread;

#define UserStackSize		1024	// increase this as necessary!
#define MAX_TOTAL_THREADS   30      // total number of threads in program lifetime
#define NUM_VIRTUAL_PAGES    (NumPhysPages) // Num of virtual pages

typedef struct id_l
{
	int id;
	void* item;
	struct id_l* next;
} *id_list;

/**
 * Thread structure inside addrspace map
 **/
enum ThreadStatusEnum
{
    THREAD_RUNNING,
    THREAD_ENDED
};

struct ThreadInfo
{
    enum ThreadStatusEnum status;
    int ret;
    Thread *ptr;
    Semaphore *join;
};

/**
 * Rights for pages
 **/
enum PageRight
{
    READONLY,
    READWRITE
};

/**
 * Open file structures
 **/
#define MAX_OPEN_FILES 10
struct openfile_s
{
    bool inUse;
    char *absoluteName;
    Thread *owner;
    OpenFile *handler;
};

class AddrSpace
{
  public:
    AddrSpace(); 	// Create an address space,
    void LoadExecutable(OpenFile * executable); // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch

	int SemaphoreCreate(char *name, int val);
	int SemaphoreP(int id);
	int SemaphoreV(int id);
	int SemaphoreDestroy(int id);

	#ifdef NETWORK
	int SocketCreate(NachosSocket** socket, SocketStatusEnum status, int remote_machine, int remote_port, int mailbox);
	int SocketDestroy(int id); 
	NachosSocket* GetSocketPointer(int id);
	#endif //NETWORK
	
    // Wrappers around StackMgr
    int FreeUserStack(unsigned int addr);
    int GetNewUserStack();

    // Wrappers around HeapMgr
    int GetHeapPage();
    int FreeHeapPage();

    // Keep track of threads inside this @ space
    std::map<unsigned int, ThreadInfo> GetThreads();
    void AttachThread(Thread *child);
    void DetachThread(Thread *child);
    void ThreadJoinP(unsigned int tid);
    void ThreadJoinV(unsigned int tid);
    Thread *GetThreadById(unsigned int tid);
    bool ThreadEnded(unsigned int tid);
    void KillAllThreads();      // Kill all threads inside current @ space
    void SetThreadReturn(unsigned int tid, int ret); // Set return for thread
    int GetThreadReturn(unsigned int tid); // Get return for thread
    unsigned int CurrentThreadNumber();
    void DecThreadNumber() { num_threads--; }
    unsigned int GetMaxTid();
    static void Exit(bool forceHalt = false);

    // Virtual page management
    void AllocatePages(unsigned int addr, unsigned int num);
    void SetPageRights(unsigned int addr, unsigned int num, enum PageRight r);
    void FreePages(unsigned int addr, unsigned int num);

    // for processes
    unsigned int GetPid(void);

    // Thread current directory (filesystem)
    int SetCurrentDirectory(const char* name);
    const char* GetCurrentDirectory();

    // Files management
    void CleanOpenFiles();
    int FileOpen(const char* filename);
    int FileClose(int id);
    int FileWrite(int id, int into, int numBytes);
    int FileRead(int id, int buffer, int numBytes);
    int FileSeek(int id, int position);
    int FileRemove(const char* filename);

    private:

    // Open file table
    openfile_s filetable[MAX_OPEN_FILES];

    TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual
    // StackMgr helpers to manage user stack
    StackMgr *stackMgr;
    // HeapMgr helpers to manage user heap
    HeapMgr *heapMgr;
    // Keep track of threads inside this @space
    std::map<unsigned int, ThreadInfo> threads;
    unsigned int max_tid;
    unsigned int num_threads; // Number of threads running currently

	id_list semaphore_list;
	unsigned int semaphore_counter;
	void CleanSemaphores();

	#ifdef NETWORK
	id_list socket_list; // Socket's list
	unsigned int socket_counter;
	void CleanSockets();
	#endif //NETWORK

    void InitTranslation();
    void CleanPageTable();
    // address space

    //For processes, set when creating the addrspace
    unsigned int pid;

    char *currentDirectory;
};

#endif // ADDRSPACE_H
