/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "memory.h"


#define MAX_ARGS            3

#define DEFAULT_PROCESS     "./process"

#define PCB_UNINITIALIZED   0
#define PCB_INITIALIZED     1
#define PCB_READY           2
#define PCB_RUNNING         3
#define PCB_SUSPENDED       4
#define PCB_TERMINATED      5

struct pcb
{
    pid_t pid;                      
    char* args[MAX_ARGS];        
    unsigned int arrival_time;
    unsigned int priority;
    unsigned int remaining_cpu_time;
    unsigned int status;

    unsigned int num_printers;
    unsigned int num_scanners;
    unsigned int num_modems;
    unsigned int num_drives;
    unsigned int mbytes;

    struct mab* mem_block;

    struct pcb* next;
};

struct pcb* create_null_pcb();
struct pcb* start_pcb(struct pcb* p);
struct pcb* suspend_pcb(struct pcb* p);
struct pcb* terminate_pcb(struct pcb* p);

struct pcb* enqueue_pcb(struct pcb* head, struct pcb* p);
struct pcb* dequeue_pcb(struct pcb* head);