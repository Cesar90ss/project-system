// heapmgr.cc
//      Routines to manage user stacks
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "heapmgr.h"
#include "stackmgr.h"
#include "system.h"
#include "addrspace.h"

/**
 * data_end_at indicate where the data segment end.
 * Suppose code & segment contiguous and code start at 0x0
 **/
HeapMgr::HeapMgr(AddrSpace *_space, unsigned int data_end_at)
{
    // Compute where heap begin
    if ((data_end_at + 1) % PageSize == 0) // Page Aligned
        heap_begin_addr = data_end_at + 1;
    else // Compute data page
        heap_begin_addr = ((data_end_at % PageSize) + 1) * PageSize;

    unsigned int total_memory_size = NUM_VIRTUAL_PAGES * PageSize;

    // Assert stack & heap will not overlapp
    ASSERT(heap_begin_addr + (HEAP_PAGES_NUMBER + MIN_PROTECTION_PAGE) * PageSize < total_memory_size - STACK_PAGES_NUMBER * PageSize);

    // At beginning, no pages for heap
    heap_end = heap_begin_addr;

    // Get reference to space for virtual memory
    this->space = _space;
}

HeapMgr::~HeapMgr()
{
    // Flush heap pages
    while (heap_end > heap_begin_addr)
        FreePage();
}

/**
 * AllocatePage gives one page for heap
 *
 * Return 0 on success, -1 if HEAP_PAGES_NUMBER has already been allocated
 **/
int HeapMgr::AllocatePage()
{
    // Check for number of page for heap already allocated
    if (heap_end >= heap_begin_addr + HEAP_PAGES_NUMBER * PageSize)
        return -1;

    // Ask for page at AddrSpace
    space->AllocatePages(heap_end, PageSize);

    // Update heap ptr
    heap_end += PageSize;

    return 0;
}

/**
 * FreePage gives back a page to AddrSpace
 **/
void HeapMgr::FreePage()
{
    // heap_end should be page aligned
    ASSERT(heap_end % PageSize == 0);

    // Heap should have at least one page (no assert, do nothing)
    if (heap_end - PageSize < heap_begin_addr)
        return;

    // Give back page to AddrSpace
    space->FreePages(heap_end - PageSize, PageSize);

    // Update heap ptr
    heap_end -= PageSize;
}
