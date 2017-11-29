/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include <stdlib.h>
#include <stdio.h>


struct mab
{
    unsigned int offset;
    unsigned int size;
    int allocated;

    struct mab* next;
};

struct mab* create_null_mab();
struct mab* search_memory(struct mab* m, unsigned int size);
struct mab* allocate_memory(struct mab* m, unsigned int size);
struct mab* free_memory(struct mab* m);