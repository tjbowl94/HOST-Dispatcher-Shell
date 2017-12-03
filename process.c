/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: process.c
**      -----------------------------
**      Source file for the process module. Contains function definitions for all
**      behavior needed for process management
**
*/


// Includes
#include "process.h"


/*
** Function: create_null_pcb()
** ---------------------------------
** Creates a new pointer to pcb struct with all values initialized to NULL/0 or default values
**
** in: void
** out: struct pcb* - pointer to newly created pcb
*/
struct pcb* create_null_pcb()
{
    struct pcb* p = (struct pcb*)malloc(sizeof(struct pcb));    // Create new pcb
    p->pid = 0; // Initialize all values to 0/NULL/default values
    p->args[0] = DEFAULT_PROCESS;
    p->args[1] = NULL;
    p->arrival_time = 0;
    p->priority = 0;
    p->remaining_cpu_time = 0;
    p->status = PCB_UNINITIALIZED;
    p->num_printers = 0;
    p->num_scanners = 0;
    p->num_modems = 0;
    p->num_drives = 0;
    p->mbytes = 0;
    p->mem_block = NULL;
    p->next = NULL;

    return p;
}

/*
** Function: start_pcb()
** ---------------------------------
** Starts a process for the first time by forking and running the process. Additionally
** updates the status of the pcb
**
** in: struct pcb* - pointer to process to be started
** out: struct pcb* - pointer to process newly started
*/
struct pcb* start_pcb(struct pcb* p)
{
    if (p->pid == 0)    // Make sure process has not been started already
    {
        switch(p->pid = fork()) // Start the process by forking
        {
            case -1:    // Handle error in the case process did not fork correctly
                // Handle error
                exit(1);
            case 0: 
                p->pid = getpid();  // Otherwise, update pcb
                p->status = PCB_RUNNING;
                // Handle printing running status
                fflush(stdout);
		execvp(p->args[0], p->args);
                exit(2);
        }
    }
    else    // If process was already started, just restart it by sending continue signal
    {
        kill(p->pid, SIGCONT);
    }
    p->status = PCB_RUNNING;
    return p;
}


/*
** Function: suspend_pcb()
** ---------------------------------
** Suspends a process that had previously been started already.
**
** in: struct pcb* - pointer to running process
** out: struct pcb* - pointer to newly suspended process
*/
struct pcb* suspend_pcb(struct pcb* p)
{
    int status;
    kill(p->pid, SIGTSTP);  // Send a stop signal to process
    waitpid(p->pid, &status, WUNTRACED);    // Wait for return from process
    p->status = PCB_SUSPENDED;  // Update pcb status
    return p;
}


/*
** Function: restart_pcb()
** ---------------------------------
** Restarts a process that was previously suspended.
**
** in: vstruct pcb* - pointer to process to be suspended
** out: struct pcb* - pointer to newly suspended process
*/
struct pcb* restart_pcb(struct pcb* p)
{
    kill(p->pid, SIGCONT);  // Send a continue signal to process
    return p;
}


/*
** Function: terminate_pcb()
** ---------------------------------
** Terminates a process. To be called when proccess is finished running.
**
** in: struct pcb* - pointer to process to be terminated
** out: struct pcb* - pointer to newly terminated process
*/
struct pcb* terminate_pcb(struct pcb* p)
{
    int status;
    kill(p->pid, SIGINT);   // Send kill signal to process
    waitpid(p->pid, &status, WUNTRACED);    // Wait for response from process
    p->status = PCB_TERMINATED; // Update status to terminated
    return p;
}


/*
** Function: enqueue_pcb()
** ---------------------------------
** Adds a process to the back of a queue
**
** in: struct pcb* - pointer to head of queue, struct pcb* - pointer to process to be added to queue
** out: struct pcb* - pointer to head of updated queue
*/
struct pcb* enqueue_pcb(struct pcb* head, struct pcb* p)
{
    struct pcb* curPcb = head;
    if (head)   // If the queue is not currently empty
    {
        while (curPcb->next)    // Find the end of the queue
        {
            curPcb = curPcb->next;  
        }
        curPcb->next = p;   // Add the process to the end of queue
        return head;
    }
    else    // If the queue is empty, just return p
    {
        return p;
    }
}


/*
** Function: dequeue_pcb()
** ---------------------------------
** Removes a process from the front of a queue.
**
** in: struct pcb* - head of queue 
** out: struct pcb* - new head of queue, with previous head removed
*/
struct pcb* dequeue_pcb(struct pcb* head)
{
    if (!head)
    {
	    return NULL;    // Return NULL if queue is empty
    }
    else
    {
        struct pcb* newHead = head->next;   // Otherwise remove head 
	    head->next = NULL;
	    return newHead; // Return new head
    }
}
