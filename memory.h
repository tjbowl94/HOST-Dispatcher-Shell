/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


struct mab
{
    unsigned int offset;
    unsigned int size;
    int allocated;

    struct mab* prev;
    struct mab* next;
};

struct mab* create_null_mab();
struct mab* fit_memory(struct mab* mem_block, unsigned int size);
struct mab* allocate_memory(struct mab* mem_block, unsigned int size);
struct mab* free_memory(struct mab* mem_block);
struct mab* split_memory(struct mab* mem_block, unsigned int size);
struct mab* merge_memory(struct mab* mem_block);