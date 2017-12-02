/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include "resources.h"


struct resources* create_null_resources()
{
    struct resources* rsrcs = (struct resources*)malloc(sizeof(struct resources));
    rsrcs->remaining_printers = 0;
    rsrcs->remaining_scanners = 0;
    rsrcs->remaining_modems = 0;
    rsrcs->remaining_drives = 0;
    return resrcs;
}

bool check_resources(struct pcb* p, struct resources* rsrcs)
{
    return (rsrcs->remaining_printers >= p->num_printers && 
        rsrcs->remaining_scanners >= p->num_scanners && 
        rsrcs->remaining_modems >= p->num_modems && 
        rsrcs->remaining_drives >= p->num_drives);
}