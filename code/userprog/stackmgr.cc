// stackmgr.cc
//      Routines to manage user stacks
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "stackmgr.h"
#include "system.h"
#include "addrspace.h"

/**
 * data_end_at indicate where the data segment end.
 * Suppose code & segment contiguous and code start at 0x0
 **/
StackMgr::StackMgr(unsigned int data_end_at)
{
    unsigned int total_memory_size;
    unsigned int total_free_size;
    unsigned int real_page_size = UserStackSize + PROTECTION_PAGE * PageSize;

    // Total memory size
    total_memory_size = NumPhysPages * PageSize;
    DEBUG('t', "Total memory size : %lu\n", total_memory_size);

    // Total memory allowed for stack
    total_free_size = total_memory_size - data_end_at - 16; // -16 : see AddrSpace::InitRegisters
    DEBUG('t', "Total memory size for stack: %lu\n", total_free_size);

    // Compute number of possible stacks
    number_of_stack = total_free_size / real_page_size;
    DEBUG('t', "Number of possible stacks: %lu\n", number_of_stack);

    // Create bitmap
    bitmap = new BitMap(number_of_stack);

    // Mark the first stack
    bitmap->Mark(0);

    // Keep first stack addr
    first_stack_addr = total_memory_size - 16;
    DEBUG('t', "Code+Data ends at: %lu\n", data_end_at);
}

StackMgr::~StackMgr()
{
    // De-alocate bitmap
    delete bitmap;
}

/**
 * GetNewStack return address in user space corresponding to user stack, NULL if
 * no more memory
 **/
unsigned int StackMgr::GetNewStack()
{
    int index;
    unsigned int stack_addr;
    unsigned int real_page_size = UserStackSize + PROTECTION_PAGE * PageSize;

    // Find the first bit which is clear
    index = bitmap->FindFirst();

    // If error, return NULL
    if (index == -1)
        return 0;

    // Compute stack addr
    stack_addr = first_stack_addr - index * real_page_size;

    // Clear stack
    bzero(machine->mainMemory + stack_addr - real_page_size, real_page_size);

    // TODO: mark PROTECTION_PAGE to trigger page-fault
    return stack_addr;
}

/**
 * FreeStack
 * address : int corresponding to the stack freed
 * return -1 if invalid addr
 **/
int StackMgr::FreeStack(unsigned int addr)
{
    unsigned int stack_index;
    unsigned int real_page_size = UserStackSize + PROTECTION_PAGE * PageSize;

    // Check if addr does not goes out memory
    if (addr > first_stack_addr &&
        addr < (first_stack_addr - (number_of_stack - 1) * real_page_size))
        return -1;

    // Check if it is the begin of a stack
    if ((first_stack_addr - addr) % real_page_size != 0)
        return -1;

    // Compute stack index
    stack_index = (first_stack_addr - addr) / real_page_size;

    // Check if stack is used
    if (!bitmap->Test(stack_index))
        return -1;

    // Clean stack
    bitmap->Clear(stack_index);

    return 0;
}

