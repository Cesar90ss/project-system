// synch.cc
//      Routines for synchronizing threads.  Three kinds of
//      synchronization routines are defined here: semaphores, locks
//      and condition variables (the implementation of the last two
//      are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"
#include "thread.h"

/***********************************************************************/
//SEMAPHORE
/***********************************************************************/

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      Initialize a semaphore, so that it can be used for synchronization.
//
//      "debugName" is an arbitrary name, useful for debugging.
//      "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------
Semaphore::Semaphore (const char *debugName, int initialValue)
{
	name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::~Semaphore
//      De-allocate semaphore, when no longer needed.  Assume no one
//      is still waiting on it!
//----------------------------------------------------------------------
Semaphore::~Semaphore ()
{
	delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//      Wait until semaphore value > 0, then decrement.  Checking the
//      value and decrementing must be done atomically, so we
//      need to disable interrupts before checking the value.
//
//      Note that Thread::Sleep assumes that interrupts are disabled
//      when it is called.
//----------------------------------------------------------------------
void Semaphore::P ()
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff);	//disable interrupts

	while (value == 0)
	{				// semaphore not available
		queue->Append ((void *) currentThread);	// so go to sleep
		currentThread->Sleep ();
	}
	value--;			// semaphore available,
    // consume its value

    (void) interrupt->SetLevel (oldLevel);	//enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//      Increment semaphore value, waking up a waiter if necessary.
//      As with P(), this operation must be atomic, so we need to disable
//      interrupts.  Scheduler::ReadyToRun() assumes that threads
//      are disabled when it is called.
//----------------------------------------------------------------------
void Semaphore::V ()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel (IntOff); //disable interrupts

    thread = (Thread *) queue->Remove ();
    if (thread != NULL)		// make thread ready, consuming the V immediately
	scheduler->ReadyToRun (thread);
    value++;
    (void) interrupt->SetLevel (oldLevel); //enable interrupts
}

/***********************************************************************/
//LOCK
/***********************************************************************/

//----------------------------------------------------------------------
// Lock::Lock
//      Initialize a lock, so that it can be used for synchronization.
//      "debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------
Lock::Lock (const char *debugName)
{
	name = debugName;
	queue = new List;
	locker = NULL;
}

//----------------------------------------------------------------------
// Lock::~Lock
//		De-allocate lock, when no longer needed.  Assume no one
//		is still waiting on it!
//----------------------------------------------------------------------
Lock::~Lock ()
{
	delete queue;
}

//----------------------------------------------------------------------
// Lock::Acquire
//		Take the lock if nobody has it or wait for the lock to be released.
//		If many threads wait for it, the first arrived will be the first
//		served.
//----------------------------------------------------------------------
void Lock::Acquire ()
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff);	//disable interrupts

    if(locker != NULL) //sleep if the lock is not free
	{
		queue->Append ((void *) currentThread);
		currentThread->Sleep();
	}

	locker = currentThread;

    (void) interrupt->SetLevel (oldLevel);	//enable interrupts
}

//----------------------------------------------------------------------
// Lock::Release
//		Give the lock to a waiting thread or release it if no other thread
//		wait for it.
//		Do nothing if the current thread does not have the lock.
//----------------------------------------------------------------------
void Lock::Release ()
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff); //disable interrupts

	if(isHeldByCurrentThread())
	{
		Thread *thread = (Thread *) queue->Remove(); //take a thread in the
													// queue if there is one
		if (thread != NULL)
		{
			scheduler->ReadyToRun(thread); //wake it up if
		}
		else
		{
			locker = NULL; //else release the lock
		}
	}

    (void) interrupt->SetLevel (oldLevel); //enable interrupts
}

//----------------------------------------------------------------------
// Lock::isHeldByCurrentThread
//		return true if the current thread has the lock, else false
//----------------------------------------------------------------------
bool Lock::isHeldByCurrentThread()
{
	return(currentThread == locker);
}

/***********************************************************************/
//CONDITION
/***********************************************************************/

//----------------------------------------------------------------------
// Condition::Condition
//      Initialize a condition, so that it can be used for synchronization.
//      "debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------
Condition::Condition (const char *debugName)
{
	name = debugName;
	queue = new List;
}

//----------------------------------------------------------------------
// Condition::~Condition
//		De-allocate condition, when no longer needed.  Assume no one
//		is still waiting on it!
//----------------------------------------------------------------------
Condition::~Condition ()
{
	delete queue;
}

//----------------------------------------------------------------------
//Condition::Wait		
//		conditionLock is automatically unlock and the thread sleep until
//		an other thread signal the condition.
//
//		Important note: The thread which call this procedure must have the
//		lock on conditionLock. If no the wait is no more atomic. 
//----------------------------------------------------------------------
void Condition::Wait (Lock * conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff); //disable interrupts

	if(conditionLock->isHeldByCurrentThread())
	{
		queue->Append ((void *) currentThread); //add the thread in the list
		conditionLock->Release(); //unlock the conditionLock
		currentThread->Sleep(); //wait for a signal
	}

    (void) interrupt->SetLevel (oldLevel); //enable interrupts
}

//----------------------------------------------------------------------
//Condition::Signal
//		Wake a thread up if there is at least one waiting on conditionLock,
//		else do nothing.
//----------------------------------------------------------------------
void Condition::Signal (Lock * conditionLock)
{
	IntStatus oldLevel = interrupt->SetLevel (IntOff); //disable interrupts

	Thread* thread = (Thread *) queue->Remove(); //take a thread in queue
	if(thread != NULL)
	{
		scheduler->ReadyToRun(thread); //wake it up if it exists
	}

    (void) interrupt->SetLevel (oldLevel);	//enable interrupts
}

//----------------------------------------------------------------------
//Condition::Broadcast
//		All the threads waiting on conditionLock are waked up.
//		Nothing happens if no threads are waiting
//----------------------------------------------------------------------
void Condition::Broadcast (Lock * conditionLock)
{
	IntStatus oldLevel = interrupt->SetLevel (IntOff); //disable interrupts

	if(conditionLock->isHeldByCurrentThread())
	{
		Thread* thread;
		while((thread=(Thread*)queue->Remove()) != NULL)
		{
			scheduler->ReadyToRun(thread); //wake every threads up
		}
	}

    (void) interrupt->SetLevel (oldLevel);	//enable interrupts
}
