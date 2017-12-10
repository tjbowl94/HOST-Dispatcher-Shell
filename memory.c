/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: memory.c
**      ----------------------------
**      Source file for the memory module. Contains function definitions for all behavior
**      needed to manage the memory of a system.
**
*/


// Includes
#include "memory.h"


/*
** Function: create_null_mab()
** ---------------------------------
** Creates a new pointer to mab struct with all values initialized to 0 or NULL
**
** in: void
** out: struct mab* - pointer to newly created mab
*/
struct mab* create_null_mab()
{
    struct mab* m = (struct mab*)malloc(sizeof(struct mab));    // Create pointer to new struct
    m->offset = 0;  // Initialize all values to 0/false/NULL
    m->size = 0;
    m->allocated = false;
    m->prev = NULL;
    m->next = NULL;
    return m;
}

/*
** Function: fit_memory()
** ---------------------------------
** Determines whether a process of a particular size can fit in a memory block. Returns
** pointer to a mab that is big enough if found, NULL if none is found. Uses the first-fit algorithm
**
** in: struct mab* - memory block to search, unsigned int size - size of new block
** out: struct mab* (if suitable mab found), NULL (if none found)
*/
struct mab* fit_memory(struct mab* mem_block, unsigned int size)
{
    struct mab* m = mem_block;
    while (m)   // Loop through the memory 'list'
    {
        if (!m->allocated && m->size >= size)
        {
            return m;
        }
        m = m->next;
    }
    return NULL;
}

/*
** Function: allocate_memory()
** ---------------------------------
** Allocates a block of memory of specified size. Returns pointer to allocated memory block if
** successfully allocated and NULL if unsuccessful
**
** in: struct mab* - memory block to search, unsigned int size - size of new block
** out: struct mab* (if successfully allocated), NULL (if not)
*/
struct mab* allocate_memory(struct mab* mem_block, unsigned int size)
{
    struct mab* m = mem_block;
    m = fit_memory(mem_block, size);    // Find where the memory can fit
    if (m)
    {
        m = split_memory(m, size);  // Split the memory into a block of needed size
        if (m)
        {
            m->allocated = true;
        }
    }
    return m;
}

/*
** Function: free_memory()
** ---------------------------------
** Opposite of allocate_memory(). Frees a particular block of memory by unallocating it and
** attempting to merge it with the previous block and next block
**
** in: struct mab* - block of memory to free
** out: struct mab* - block of memory that has been freed
*/
struct mab* free_memory(struct mab* mem_block)
{
    mem_block->allocated = false;
    merge_memory(mem_block);
    merge_memory(mem_block->prev);
}

/*
** Function: split_memory()
** ---------------------------------
** Splits a memory block into two memory blocks, where the first block is of the desired size. Returns
** pointer to first block if successful and NULL if unsuccessful
**
** in: struct mab* - memory block to search, unsigned int size - size in mbytes of desired block
** out: struct mab* (if successfully split), NULL (if not)
*/
struct mab* split_memory(struct mab* mem_block, unsigned int size)
{
    if (!mem_block->allocated && size <= mem_block->size)   // Make sure block is unallocated and size is large enough
    {
        if (mem_block->size > size) // If size is bigger than desired size, split the block
        {
            struct mab* new_mem_block = create_null_mab();
            new_mem_block->offset = mem_block->offset + size;
            new_mem_block->size = mem_block->size - size;
            mem_block->size = size;
            new_mem_block->next = mem_block->next;
            mem_block->next = new_mem_block;
            new_mem_block->prev = mem_block;
        }
        return mem_block;   // If the size is exact, no need to split. Just return the block
    }
    return NULL;
}

/*
** Function: merge_memory()
** ---------------------------------
** Attempts to merge a block of memory with the next block. Can only do so if both blocks are unallocated. Returns
** pointer to new merged block if successful, pointer to unmerged block if unsuccessful
**
** in: struct mab* - pointer to block to attempt to merge with next
** out: struct mab* - pointer to newly merged block (if successful) or same block (if unsuccessful)
*/
struct mab* merge_memory(struct mab* mem_block)
{
    if (mem_block && !mem_block->allocated && mem_block->next && !mem_block->next->allocated)   // Check to make sure both this block and next are unallocated
    {
        struct mab* next_mem_block = mem_block->next;   // If so, merge the two together
        mem_block->size += next_mem_block->size;
        mem_block->next = next_mem_block->next;
        if (next_mem_block->next)
        {
            next_mem_block->next->prev = mem_block;
        }
    }
    return mem_block;
}
