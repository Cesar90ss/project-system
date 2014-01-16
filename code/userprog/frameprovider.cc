// stackmgr.cc
//      Routines to manage user stacks
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "frameprovider.h"
#include "system.h"
#include "addrspace.h"

/**
 * data_end_at indicate where the data segment end.
 * Suppose code & segment contiguous and code start at 0x0
 **/
FrameProvider::FrameProvider()
{
    total_frame_number=NumPhysPages;
    number_of_free_frame = total_frame_number-1;
    // Create bitmap
    bitmap = new BitMap(total_frame_number);
    
    bitmap->Mark(0);

}

FrameProvider::~FrameProvider()
{
    // De-alocate bitmap
    delete bitmap;
}

/**
 * GetNewFrameProvider return address in user space corresponding to Physical Frame, NULL if
 * no more memory
 **/
unsigned int FrameProvider::GetEmptyFrame()
{
    int index;
    unsigned int frame_addr;
    // Find the first bit which is clear
    index = bitmap->Find();

    // If error, return NULL
    if (index == -1)
        return 0;

    // Compute Physical addr
    frame_addr = 1+index*PageSize;

    // Clear frame
    bzero(machine->mainMemory + frame_addr,PageSize);
    
		number_of_free_frame--;

    return frame_addr;
}

/**
 * FreeFrame
 * address : int corresponding to the frame freed
 * return -1 if invalid addr
 **/
int FrameProvider::ReleaseFrame(unsigned int addr)
{
    unsigned int frame_index;
   
    // Check if addr does not goes out memory
    if (addr > MemorySize-1 &&
        addr < 0 )
        return -1;

    // Check if it is the begin of a frame
    if ((addr) % PageSize != 0)
        return -1;

    // Compute frame index
    frame_index = addr / PageSize;

    // Check if frame is used
    if (!bitmap->Test(frame_index))
        return -1;

    // Clean frame
    bitmap->Clear(frame_index);
    
    number_of_free_frame++;
    
    return 0;
}

int FrameProvider::NumAvailFrame()
{
  return number_of_free_frame;
}

