// processmgr.h
//      Routines to manage user process
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef PROCESSMGR_H
#define PROCESSMGR_H
#include "copyright.h"
class AddrSpace;
#include "synch.h"

#include <map>

enum ProcessStatusEnum
{
    PROCESS_RUNNING,
    PROCESS_WAITING,	//after waitpid
    PROCESS_ENDED
};

struct ProcessInfo
{
    enum ProcessStatusEnum status;
    AddrSpace *space;
    Semaphore *wait;  //for waitpid
};

class ProcessMgr {
public:
  
  ProcessMgr(); 
  ~ProcessMgr(); //destroy the map
  void KillAllProcess();
  unsigned int CreateProcess(AddrSpace *space);
  void EndProcess(AddrSpace *proc);
  void ProcessWaitP(unsigned int pid);
  void ProcessWaitV(unsigned int pid);
  AddrSpace *GetProcessById(unsigned int pid);
  bool ProcessEnded(unsigned int pid);
  
  std::map<unsigned int, ProcessInfo> getProcess();
private:
  //keep tracks of the processes
  std::map<unsigned int, ProcessInfo> Processes;
  //max pid that was given
  unsigned int max_pid;
  //number of processes currently running
  unsigned int nbProcess;
};
#endif