/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: resources.c
**      --------------------------------
**      Source file for the resources module. Contains function definitions for all behavior
**      needed to manage the resources of a system. 
**
*/


// Includes
#include "resources.h"


/*
** Function: create_null_resources()
** ---------------------------------
** Creates a new pointer to resources struct with all values initialized to 0
**
** in: void
** out: struct resources*
*/
struct resources* create_null_resources()
{
    struct resources* rsrcs = (struct resources*)malloc(sizeof(struct resources));  // Create pointer to new struct
    rsrcs->remaining_printers = 0;  // Initialize all values to 0
    rsrcs->remaining_scanners = 0;
    rsrcs->remaining_modems = 0;
    rsrcs->remaining_drives = 0;
    return rsrcs;
}

/*
** Function: check_resources()
** ---------------------------------
** Checks the resources required by a process against the resources left in the system
** to determine whether there are sufficient resources available to complete the process
**
** in: struct pcb*, struct resources*
** out: bool
*/
bool check_resources(struct pcb* p, struct resources* rsrcs)
{
    return (rsrcs->remaining_printers >= p->num_printers && 
        rsrcs->remaining_scanners >= p->num_scanners && 
        rsrcs->remaining_modems >= p->num_modems && 
        rsrcs->remaining_drives >= p->num_drives);
}

/*
** Function: allocate_resources()
** ---------------------------------
** Allocates the resources required by a particular process to that process by subtracting
** the corresponding number of resources from the count of the system.
**
** in: struct pcb*, struct resources*
** out: void
*/
void allocate_resources(struct pcb* p, struct resources* rsrcs)
{
    rsrcs->remaining_printers -= p->num_printers;
    rsrcs->remaining_scanners -= p->num_scanners;
    rsrcs->remaining_modems -= p->num_modems;
    rsrcs->remaining_drives -= p->num_drives;
}

/*
** Function: free_resources()
** ---------------------------------
** Frees the resources required by a particular process from that process by adding
** the corresponding number of resources back into the count of the system
**
** in: struct pcb*, struct resources*
** out: void
*/
void free_resources(struct pcb* p, struct resources* rsrcs)
{
    rsrcs->remaining_printers += p->num_printers;
    rsrcs->remaining_scanners += p->num_scanners;
    rsrcs->remaining_modems += p->num_modems;
    rsrcs->remaining_drives += p->num_drives;
}