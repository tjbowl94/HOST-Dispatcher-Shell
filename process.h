/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: process.h
**      ----------------------------
**      Header file for the process module. Contains definition of useful constants,
**      definition for the pcb struct, as well as function declarations for the 
**      functions used to manage processes. Function definitions and more extensive 
**      comments can be found in the process.c file
**
*/


// Includes
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "memory.h"


// The argument size for a particular process
#define MAX_ARGS            3

// The process that will run for each new process (source code in sigtrap.c)
#define DEFAULT_PROCESS     "./process"

// Status codes for each process
#define PCB_UNINITIALIZED   0
#define PCB_INITIALIZED     1
#define PCB_READY           2
#define PCB_RUNNING         3
#define PCB_SUSPENDED       4
#define PCB_TERMINATED      5

/*
** The pcb struct represents the process control block for each new process. It contains a collection
** of data representing the control block, including process id, arrival time, priority, remaining cpu time,
** status, number of required resources, pointer to memory block, and pointer to next pcb (if the process)
** is currently in a queue
*/
struct pcb
{
    pid_t pid;  // Process id 
    char* args[MAX_ARGS];   // Process arguments    
    unsigned int arrival_time;  // Time process arrives in queue
    unsigned int priority;  // Priority of process (0 for real-time, 1-3 for user job)
    unsigned int remaining_cpu_time;    // Time left until process is completed
    unsigned int status;    // Current status of process

    unsigned int num_printers;  // Number of printers required by process
    unsigned int num_scanners;  // Number of scanners required by process
    unsigned int num_modems;    // Number of modems required by process
    unsigned int num_drives;    // Number of drives required by process
    unsigned int mbytes;    // Memory requirement of process in mbytes

    struct mab* mem_block;  // Pointer to memory block for process (if it has been allocated)

    struct pcb* next;   // Pointer to next process (if in queue)
};

struct pcb* create_null_pcb();  // Creates a pointer to new pcb with all values initialized to null/0
struct pcb* start_pcb(struct pcb* p);   // Starts a process
struct pcb* suspend_pcb(struct pcb* p); // Suspends a process
struct pcb* restart_pcb(struct pcb* p); // Restarts a previously-suspended process
struct pcb* terminate_pcb(struct pcb* p);   // Terminates a process

struct pcb* enqueue_pcb(struct pcb* head, struct pcb* p);   // Adds a process to a queue
struct pcb* dequeue_pcb(struct pcb* head);  // Removes the front of a process queue 
