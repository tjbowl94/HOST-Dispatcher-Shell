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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "resources.h"


#define TOTAL_MEMORY    1024

#define TOTAL_PRINTERS  2
#define TOTAL_SCANNERS  1
#define TOTAL_MODEMS    1
#define TOTAL_DRIVES    2


void print_usage();
void fill_input_queue(char* input_file, FILE* input_list_stream, struct pcb* input_queue);
void initialize_system(struct mab* mem, struct resources* rsrcs);
bool complete(struct pcb* i_q, struct pcb* r_t_q, struct pcb* u_j_q, struct pcb* p_one_q, struct pcb* p_two_q, struct pcb* p_three_q, struct pcb* cur_p);