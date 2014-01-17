// stackmgr.h
//      Routines to manage user stacks
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FRMPRVD_H
#define FRMPRVD_H

#include "copyright.h"
#include "bitmap.h"

// Definitions of allocation strategy types
#define FIRST	0
#define LAST	1
#define RANDOM	2

class FrameProvider
{
    public:
    FrameProvider(int allocation_strategy);  // Init bitmap associated to physical address space
    ~FrameProvider(); // De-allocate bitmap

    // Return address of new Physical address
    int GetEmptyFrame(unsigned int *page); 
    
    // Mark a Physical address as freed
    int ReleaseFrame(unsigned int addr);
    
    //Return number of available frame 
    int NumAvailFrame();

    private:
    BitMap *bitmap;
    unsigned int number_of_free_frame;
    unsigned int total_frame_number;
	int alloc_strat;
};

#endif // FRMPRVD_H
