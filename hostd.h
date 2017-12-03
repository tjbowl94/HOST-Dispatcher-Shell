/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include <errno.h>
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

// Global variables
struct pcb* input_queue = NULL;
struct pcb* real_time_queue = NULL;
struct pcb* user_job_queue = NULL;
struct pcb* priority_one_queue = NULL;
struct pcb* priority_two_queue = NULL;
struct pcb* priority_three_queue = NULL;

struct pcb* current_process = NULL;
struct pcb* process = NULL;

struct mab* memory = NULL;

struct resources* rsrcs = NULL;

unsigned int timer = 0;

void print_usage();
void fill_input_queue(char* input_file, FILE* input_list_stream);
void initialize_system();
bool complete();