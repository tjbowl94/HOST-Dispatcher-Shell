/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: hostd.h
**	---------------------------
**	Header file for hostd.c, where most of the logic of the main algorithm takes place.
**	This header file contains all appropriate includes, as well as definitions for important
**	constants (these constants define the constraints of the system in terms of memory and
**	resources), as well as global variables used in hostd.c and function declarations
**
*/


// Includes
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


// Memory constraints of the system
#define RESERVED_MEMORY	64	// Amount of memory reserved for real-time processes
#define TOTAL_MEMORY    1024	// Total amount of memory in the system

// Resource constraints of the system
#define TOTAL_PRINTERS  2	// Number of printers the system has access to
#define TOTAL_SCANNERS  1	// Number of scanners the system has access to
#define TOTAL_MODEMS    1	// Number of modems hte system has access to
#define TOTAL_DRIVES    2	// Number of drives the system has access to

// Global variables
struct pcb* input_queue = NULL;	// Pointer to the input queue, where all processes go after being scanned in
struct pcb* real_time_queue = NULL;	// Pointer to the real-time queue, which contains all real-time processess yet to be completed
struct pcb* user_job_queue = NULL;	// Pointer to the user job queue, which contains all user jobs yet to be started
struct pcb* priority_one_queue = NULL;	// Pointer to priority one queue
struct pcb* priority_two_queue = NULL;	// Pointer to priority two queue
struct pcb* priority_three_queue = NULL;	// Pointer to priority three queue

struct pcb* current_process = NULL;	// Pointer to the currently running process
struct pcb* process = NULL;	// Auxiliary process pointer to be used when creating new process

struct mab* memory = NULL;	// Pointer to the main memory block
struct mab* reserved_memory = NULL;	// Pointer to reserved real-time job memory block

struct resources* rsrcs = NULL;	// Pointer to the struct containing number of system resources currently held

unsigned int timer = 0;	// Timer for the whole system

void print_usage();	// Print the correct usage of the program when it is attempted to run without input file
void fill_input_queue(char* input_file, FILE* input_list_stream);	// Fill the input queue based on input file
void initialize_system();	// Initialize essential components of the system, such as memory and resource info
bool complete();	// Check if the system has completed all processes
void check_input_queue();	// Remove processes from input queue and place in appropriate queue
void check_user_job_queue();	// Remove processes from user job queue and place in appropriate queue
void handle_current_process();	// Does work on current process, and handles terminating or suspending the process if necessary
void assign_current_process();	// Assigns a new process to be worked on
