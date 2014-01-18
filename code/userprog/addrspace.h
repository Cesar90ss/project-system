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
#include "synch.h"

#include <map>

class Thread;

#define UserStackSize		1024	// increase this as necessary!
#define MAX_TOTAL_THREADS   20      // total number of threads in program lifetime
#define NUM_VIRTUAL_PAGES    (NumPhysPages) // Num of virtual pages
typedef struct slist
{
	int id;
	Semaphore *sem;
	struct slist* next;
} *sem_list;

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

class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch

	int CreateSemaphore(char *name, int val);
	int SemaphoreP(int id);
	int SemaphoreV(int id);
	int SemaphoreDestroy(int id);

    // Wrappers around StackMgr
    int FreeUserStack(unsigned int addr);
    int GetNewUserStack();

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
    unsigned int GetMaxTid();
    static void Exit(bool forceHalt = false);

    // Virtual page management
    void AllocatePages(unsigned int addr, unsigned int num);
    void SetPageRights(unsigned int addr, unsigned int num, enum PageRight r);
    void FreePages(unsigned int addr, unsigned int num);

    static unsigned int nbProcess;
  private:
    TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual
    // StackMgr helpers to manage user stack
    StackMgr *stackMgr;
    // Keep track of threads inside this @space
    std::map<unsigned int, ThreadInfo> threads;
    unsigned int max_tid;
    unsigned int num_threads; // Number of threads running currently

	sem_list semaphore_list;
	unsigned int semaphore_counter;
	void CleanSemaphores();

    void InitTranslation();
    void CleanPageTable();
    // address space
};

#endif // ADDRSPACE_H
