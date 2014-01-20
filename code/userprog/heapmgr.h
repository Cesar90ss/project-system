// heapmgr.h
//      Routines to manage user heap
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef HEAPMGR_H
#define HEAPMGR_H

#include "copyright.h"

// Num of page after heap at minimum (to trigger page fault)
#define MIN_PROTECTION_PAGE 1
// Number of page inside virtual @ space reserved for heap
#define HEAP_PAGES_NUMBER 512

class AddrSpace;

class HeapMgr
{
    public:
    HeapMgr(AddrSpace *space, unsigned int data_end_at);  // Init heap structure
    ~HeapMgr(); // De-allocate heap structure

    int AllocatePage(); // Return address of a new page (at end of heap)
    int FreePage(); // Delete the page at end of heap

    private:
    AddrSpace *space;
    unsigned int heap_begin_addr;
    unsigned int heap_end;
};

#endif // HEAPMGR_H
