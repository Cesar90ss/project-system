#include "copyright.h"
#include "processmgr.h"
#include "system.h"
#include "addrspace.h"
ProcessMgr::ProcessMgr()
{
  max_pid = 0;
  nbProcess = 0;
}

ProcessMgr::~ProcessMgr()
{
  /**
  * Delete last semaphore
  **/
  std::map<unsigned int, ProcessInfo> copy(Processes);
  std::map<unsigned int, ProcessInfo>::iterator it;
  struct ProcessInfo p;

  for (it = copy.begin(); it != copy.end(); ++it)
  {
    p = it->second;

    if (p.wait != NULL)
      delete p.wait;
  }

    Processes.clear();
}


//------------------------------------------------------------//
/**
 * Mark all processes as finished when exit
 **/
void ProcessMgr::KillAllProcess()
{
    /**
     * Each process will call EndProcess, which modify Processess
     * So make a copy
     **/
    std::map<unsigned int, ProcessInfo> copy(Processes);
    std::map<unsigned int, ProcessInfo>::iterator it;
    struct ProcessInfo p;

    for (it = copy.begin(); it != copy.end(); ++it)
    {
        p = it->second;

        // Not the current as we need it for last context switch
        if (p.space == currentThread->space)
            continue;

        // Not thread marked as ended
        if (p.status == PROCESS_ENDED)
            continue;
	EndProcess(p.space);	//end the process, clear the structure
        delete p.space; //delete the space attached to the process
    }
}
//------------------------------------------------------------//
/**
 * Suppose not inside Process
 **/
unsigned int ProcessMgr::CreateProcess(AddrSpace *space) //will be called by AddrSpace()
{
    struct ProcessInfo pi;

    pi.status = PROCESS_RUNNING;
    pi.space = space;
    pi.wait = new Semaphore("wait semaphore process", 0);

    // Insert with new pid
    Processes[max_pid++] = pi;

    nbProcess++;
    return (max_pid-1);
}
//------------------------------------------------------------//
/**
 * to clean up the structures
 **/
void ProcessMgr::EndProcess(AddrSpace *proc)
{
    struct ProcessInfo pi;
    pi = Processes[proc->GetPid()];
    pi.status = PROCESS_ENDED;
    Processes[proc->GetPid()] = pi;
    nbProcess--;
}
//------------------------------------------------------------//
/**
 * wait P on process, called by waitpid
 **/
void ProcessMgr::ProcessWaitP(unsigned int pid)
{
    if (Processes.find(pid) == Processes.end())
        return;

    Processes[pid].wait->P();	//wait on the process end
}
//------------------------------------------------------------//
/**
 * wait V on process, called when process exit
 **/
void ProcessMgr::ProcessWaitV(unsigned int pid)
{    
  if (Processes.find(pid) == Processes.end())
        return;

    Processes[pid].wait->V(); //release the semaphore
}
//------------------------------------------------------------//
/**
 * Get process(addrspace) by pid
 * Return NULL if non existence
 **/
AddrSpace *ProcessMgr::GetProcessById(unsigned int pid)
{
    if (Processes.find(pid) == Processes.end())
        return NULL;

    return Processes[pid].space;
}
//------------------------------------------------------------//
/**
 * Check if a process is ended previously
 **/
bool ProcessMgr::ProcessEnded(unsigned int pid)
{
    if (Processes.find(pid) == Processes.end())
        return false;

    return Processes[pid].status == PROCESS_ENDED;
}
//------------------------------------------------------------//