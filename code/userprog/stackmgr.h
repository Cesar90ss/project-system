// stackmgr.h
//      Routines to manage user stacks
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef STACKMGR_H
#define STACKMGR_H

#include "copyright.h"
#include "bitmap.h"

class StackMgr
{
    public:
    StackMgr(unsigned int data_end_at);  // Init bitmap associated to stack
    ~StackMgr(); // De-allocate btimap

    unsigned int GetNewStack(); // Return address of new stack
    int FreeStack(unsigned int addr); // Mark a stack as freed

    private:
    BitMap *bitmap;
    unsigned int first_stack_addr;
    unsigned int number_of_stack;
};

#endif // STACKMGR_H
