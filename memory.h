/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: memory.h
**      ----------------------------
**      Header file for the memory module. Contains the definition for the mab struct,
**      representing a block of memory. Also contains declarations for all functions 
**      used in the memory module. Function definitions, and more extensive comments can
**      be found in the memory.c file
**
*/


// Includes
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


/*
** The mab struct represents a block of memory. It contains an offset (representing its location), 
** a size, an indicator whether or not the memory is allocated to a process, and links to the previous
** and next blocks of memory. Memory is split recursively, meaning a single block of memory can be split
** up into multiple smaller blocks of memory, and can later be merged back together. 
*/
struct mab
{
    unsigned int offset;    // Location of mab in relation to original mab
    unsigned int size;  // Size of this mab in mbytes
    int allocated;  // Whether or not this mab has been allocated to a process

    struct mab* prev;   // A pointer to the previous mab in the list
    struct mab* next;   // A pointer to the next mab in the list
};

struct mab* create_null_mab();  // Create a pointer to a 0/null initialized mab struct
struct mab* fit_memory(struct mab* mem_block, unsigned int size);   // Find where the memory can fit in a mab
struct mab* allocate_memory(struct mab* mem_block, unsigned int size);  // Allocate a block to a process of a particular size
struct mab* free_memory(struct mab* mem_block); // Free the memory of a particular block
struct mab* split_memory(struct mab* mem_block, unsigned int size); // Split a block into smaller blocks
struct mab* merge_memory(struct mab* mem_block);    // Merge this block and the next block together if possible