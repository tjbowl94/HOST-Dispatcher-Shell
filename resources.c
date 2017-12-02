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


void create_null_resources(struct resources* rsrcs)
{
    rsrcs->remaining_printers = 0;
    rsrcs->remaining_scanners = 0;
    rsrcs->remaining_modems = 0;
    rsrcs->remaining_drives = 0;
}

bool check_resources(struct pcb* p, unsigned int r_printers, unsigned int r_scanners, unsigned int r_modems, unsigned int r_drives)
{
    return (r_printers >= p->num_printers && r_scanners >= p->num_scanners && r_modems >= p->num_modems && r_drives >= p->num_drives);
}