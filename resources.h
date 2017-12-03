/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: resources.h
**      --------------------------------
**      Header file for the resources module. Contains the definition for the resources struct,
**      as well as declarations for all functions used in the resources module. Function definitions,
**      and more extensive comments can be found in the resources.c file
**
*/


// Includes
#include <stdbool.h>
#include "process.h"


/*
**  The resources struct represents a collection of I/O resources contained by a system. As
**  resources are used by a process, the count of the respective resources is decremented. When 
** a process is finished, the count is incremented.
*/
struct resources
{
    unsigned int remaining_printers;    // The number of remaining printers within the system
    unsigned int remaining_scanners;    // The number of remaining scanners within the system
    unsigned int remaining_modems;  // The number of remaining modems within the system
    unsigned int remaining_drives;  // The number of remaining CD drives within the system
};

struct resources* create_null_resources();  // Create a pointer to a 0-initialized resources struct
bool check_resources(struct pcb* p, struct resources* rsrcs);   // Check that a system has enough resources to work on a process
void allocate_resources(struct pcb* p, struct resources* rsrcs);    // Allocate resources to a particular process
void free_resources(struct pcb* p, struct resources* rsrcs);    // Free the resources used by a particular process