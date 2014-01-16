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



class FrameProvider
{
    public:
    FrameProvider();  // Init bitmap associated to physical address space
    ~FrameProvider(); // De-allocate bitmap

    unsigned int GetEmptyFrame(); // Return address of new Physical address
    int ReleaseFrame(unsigned int addr); // Mark a Physical address as freed
    int NumAvailFrame();//Return number of available frame 

    private:
    BitMap *bitmap;
    unsigned int number_of_free_frame;
    unsigned int total_frame_number;
};

#endif // FRMPRVD_H
