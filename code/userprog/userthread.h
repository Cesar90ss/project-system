/* userthread.h
 * 	Nachos system call interface.  These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel.
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation
 * of liability and disclaimer of warranty provisions.
 */

#ifndef USERTHREAD_H
#define USERTHREAD_H

void StartUserThread(int f);

int do_UserThreadCreate(int fnWrapper, int fnUser, int arg);
void do_UserThreadExit();
int do_UserThreadJoin();
void StartProc(int filename);

#endif
