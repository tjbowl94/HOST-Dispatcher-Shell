/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include "memory.h"

struct mab* create_null_mab()
{
    struct mab* m = (struct mab*)malloc(sizeof(struct mab));
    m->offset = 0;
    m->size = 0;
    m->allocated = false;
    m->prev = NULL;
    m->next = NULL;
    return m;
}

struct mab* fit_memory(struct mab* mem_block, unsigned int size)
{
    struct mab* m = mem_block;
    while (m)
    {
        if (!m->allocated && m->size >= size)
        {
            return m;
        }
        m = m->next;
    }
    return NULL;
}

struct mab* allocate_memory(struct mab* mem_block, unsigned int size)
{
    struct mab* m = mem_block;
    m = fit_memory(mem_block, size);
    if (m)
    {
        m = split_memory(m, size);
        if (m)
        {
            m->allocated = true;
        }
    }
    return m;
}

struct mab* free_memory(struct mab* mem_block)
{
    mem_block->allocated = false;
    merge_memory(mem_block);
    merge_memory(mem_block->prev);
}

struct mab* split_memory(struct mab* mem_block, unsigned int size)
{
    if (!mem_block->allocated && size <= mem_block->size)
    {
        if (mem_block->size > size)
        {
            struct mab* new_mem_block = create_null_mab();
            new_mem_block->offset = mem_block->offset + size;
            new_mem_block->size = mem_block->size - size;
            mem_block->size = size;
            new_mem_block->next = mem_block->next;
            mem_block->next = new_mem_block;
            new_mem_block->prev = mem_block;
        }
        return mem_block;
    }
    return NULL;
}

struct mab* merge_memory(struct mab* mem_block)
{
    if (mem_block && !mem_block->allocated && mem_block->next && !mem_block->next->allocated)
    {
        struct mab* next_mem_block = mem_block->next;
        mem_block->size += next_mem_block->size;
        mem_block->next = next_mem_block->next;
        if (next_mem_block->next)
        {
            next_mem_block->next->prev = mem_block;
        }
    }
    return mem_block;
}