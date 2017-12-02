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
#include "process.h"


struct resources
{
    unsigned int remaining_printers;
    unsigned int remaining_scanners;
    unsigned int remaining_modems;
    unsigned int remaining_drives;  
};

void create_null_resources();
bool check_resources(struct pcb* p, struct resources* rsrcs);
