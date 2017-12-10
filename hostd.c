/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: hostd.c
**	----------------------------
**	Source file for the host dispatcher, includes most of the core logic that powers the system.
**	Contains definitions for functions used in this file, as well as the main() function which contains
**	the logic for running the program.
*/


// Includes
#include "hostd.h"

/*
** Function: main()
** ---------------------------------
** main() function - contains the core logic that runs the dispatcher system. First parses a file for a list
** of processes as specified by the user. Then handles all these processes by continually looping until all
** processes have been completed. All processes are placed in the appropriate queues, then worked in the 
** correct order.
**
*/
int main(int argc, char* argv[])
{
    char* input_file;
    FILE* input_list_stream;

    if (argc == 2)
    {
        input_file = argv[1];	// Parse the command from running the program to get input file
    }
    else
    {	
        print_usage();	// If the user did not specify a file, print the correct usage of the program, then exit
        exit(0);	
    }

    if (!(input_list_stream = fopen(input_file, "r")))	// Open the file
    {
        printf("Error opening file");	// If there was an error opening the file, print error and exit program
        exit(0);
    }

    fill_input_queue(input_file, input_list_stream);	// Fill the input queue with new processes
    initialize_system();	// Initialize the system
    
    while (!complete())	// Loop until all processes have finished
    {
        check_input_queue();	// Remove processes from input queue and put in appropriate queue

	check_user_job_queue();	// Remove processes from user job queue and put in appropriate queue

        if (current_process)    // There is a process currently running, so handle it
        {
   	    handle_current_process();
        }

        if ((real_time_queue || priority_one_queue || priority_two_queue || priority_three_queue) && (!current_process))	// There are still jobs left to be completed
        {
	    assign_current_process();
        }

        sleep(1);	// Sleep 1 second to simulate 1 second of work
        timer += 1;	// Add 1 second to the current time
    }

    printf("All processes have finished.\n");

    return 0;
}

/*
** Function: print_usage()
** ---------------------------------
** Prints the correct usage of the program when the user attempts to run the program without
** specifying an input file
**
** in: void
** out: void - just a print message to console
*/
void print_usage()
{
    printf("\nPlease supply a file specifying a sequence of processes as an argument when running this file.\n\n");
}

/*
** Function: fill_input_queue()
** ---------------------------------
** Reads a file, parses it and interprets each line as a process based on a set of parameters. Then filles the
** input queue based on these newly created processes
**
** in: file containing list of processes
** out: void - fills input queue with processes
*/
void fill_input_queue(char* input_file, FILE* input_list_stream)
{
    struct pcb* process;	// Pointer to new process to create
    char line[50];	// Line buffer

    while (fgets(line, 50, input_list_stream))	// Loop through the file line by line
    {
	if (line[0] == '\n')	// Exit loop when file ends
	{
	    break;
	}
	
        process = create_null_pcb();	// Create null process

        char* s = strtok(line, ",");	// Parse the line for eight parameters of process
        int nums[8];
        int i = 0;

        while (s)
        {
            int n = atoi(s);
            s = strtok(NULL, ",");
            nums[i] = n;
            ++i;
        }

        process->arrival_time = nums[0];	// Assign process attributes accordingly
        process->priority = nums[1];
        process->remaining_cpu_time = nums[2];
        process->mbytes = nums[3];
        process->num_printers = nums[4];
        process->num_scanners = nums[5];
        process->num_modems = nums[6];
        process->num_drives = nums[7];

        input_queue = enqueue_pcb(input_queue, process);	// Add process to input queue

        process = NULL;
    }

    fclose(input_list_stream);	// Close the file
}

/*
** Function: initialize_system()
** ---------------------------------
** Assigns values to global variables based on parameters of the system as specified. 
** Assigns memory and resources
**
** in: void
** out: void - assigns values to global variables
*/
void initialize_system()
{
    memory = create_null_mab();	// Create main memory block
    memory->size = TOTAL_MEMORY - RESERVED_MEMORY;
    reserved_memory = create_null_mab();	// Create reserved memory block
    reserved_memory->size = RESERVED_MEMORY;
    rsrcs = create_null_resources();	// Create resources
    rsrcs->remaining_printers = TOTAL_PRINTERS;
    rsrcs->remaining_scanners = TOTAL_SCANNERS;
    rsrcs->remaining_modems = TOTAL_MODEMS;
    rsrcs->remaining_drives = TOTAL_DRIVES;
}

/*
** Function: complete()
** ---------------------------------
** Checks whether the system has completed all processes by checking if all the queues are empty
**
** in: void
** out: bool - whether or not the system has finished its job
*/
bool complete()
{
    return (!input_queue && !real_time_queue && !user_job_queue && !priority_one_queue 
        && !priority_two_queue && !priority_three_queue && !current_process);
}

/*
** Function: check_input_queue()
** ---------------------------------
** Checks the input queue. If there are any processes in input queue whose arrival time is equal
** to the current time, then they are removes from the queue and placed into the appropriate queue
**
** in: void
** out: void - processes are removed from input queue and placed in appropriate queue
*/
void check_input_queue()
{
    while (input_queue && input_queue->arrival_time <= timer)	// Check for processess arriving at current time
    {
        struct pcb* p = input_queue;
	input_queue = dequeue_pcb(input_queue);	// Remove from input queue
        if (p->priority == 0)	
        {
            real_time_queue = enqueue_pcb(real_time_queue, p);	// Place in real-time queue
        }
        else
        {
            user_job_queue = enqueue_pcb(user_job_queue, p); 	// Place in user job queue
        }
    }
}

/*
** Function: check_user_job_queue()
** ---------------------------------
** Checks the user job queue. If there are any processes in user job queue and a sufficient
** amount of resources are available, the process in placed in the appropriate priority queue
**
** in: void
** out: void - processes are removed from user job queue and placed in appropriate priority queue
*/
void check_user_job_queue()
{
    while(user_job_queue && fit_memory(memory, user_job_queue->mbytes) &&	// check user job queue and make sure resources are sufficient
    check_resources(user_job_queue, rsrcs))
    {
        struct pcb* p = user_job_queue;	
	user_job_queue = dequeue_pcb(user_job_queue);	// Remove from user job queue
        allocate_resources(p, rsrcs);	// Allocate resources to the process
        switch(p->priority)	// Place in appropriate priority queue
        {
            case 1:
                priority_one_queue = enqueue_pcb(priority_one_queue, p);
                break;
            case 2:
                priority_two_queue = enqueue_pcb(priority_two_queue, p);
                break;
            default:
                priority_three_queue = enqueue_pcb(priority_three_queue, p);
                break;
        }
    }
}

/*
** Function: handle_current_process()
** ---------------------------------
** Decrement time remaining on the current process. If it has finished, terminate the process and free
** up any resources used by that process. If it hasn't finished, decrease the priority and place it in
** appropriate priority queue (if it isn't real-time job)
**
** in: void
** out: void - current process parameters have been changed
*/
void handle_current_process()
{
    if (--current_process->remaining_cpu_time == 0) // Check if the process has finished
    {
        terminate_pcb(current_process);	// Terminate process
        free_memory(current_process->mem_block);	// Free process memory
        free_resources(current_process, rsrcs);	// Free process resources
        free(current_process);
        current_process = NULL;
    }
    else    // The process hasn't finished
    {   
        if ((real_time_queue || user_job_queue || priority_one_queue || priority_two_queue || priority_three_queue) &&    
        (current_process->priority != 0))     // There are still jobs in the queue(s)
        {
            struct pcb* p = suspend_pcb(current_process);	// Suspend current process
            if (++p->priority > 3)	// Reduce the priority of the process
            {
                p->priority = 3;
            }
            switch(p->priority)	// Place process in appropriate priority queue
            {
                case 1:
                    priority_one_queue = enqueue_pcb(priority_one_queue, p);
                    break;
                case 2:
                    priority_two_queue = enqueue_pcb(priority_two_queue, p);
                    break;
                default:
                    priority_three_queue = enqueue_pcb(priority_three_queue, p);
                    break;
            }
            current_process = NULL;	// Reset pointer to NULL since process has been suspended
        }
    }
}

/*
** Function: assign_current_process()
** ---------------------------------
** Called when there are no processes being worked on. So check the queues in the appropriate order 
** (real-time then p1 then p2 then p3) and the highest-priority process is assigned to be the 
** new currently running process
**
** in: void
** out: void - current process has been assigned
*/
void assign_current_process()
{
    if (real_time_queue)	// Check real-time queue first
    {
        current_process = real_time_queue;
	real_time_queue = dequeue_pcb(real_time_queue);
    }
    else if (priority_one_queue)	// Then check priority one queue
    {
        current_process = priority_one_queue;
	priority_one_queue = dequeue_pcb(priority_one_queue);
    }
    else if (priority_two_queue)	// Then check priority two queue
    {
        current_process = priority_two_queue;
	priority_two_queue = dequeue_pcb(priority_two_queue);
    }
    else	// Otherwise, assign from priority three queue
    {
        current_process = priority_three_queue;
	priority_three_queue = dequeue_pcb(priority_three_queue);
    }

    if (current_process->pid != 0)	// Check if the process has already been started before
    {
        restart_pcb(current_process);	// If so, just restart it
    }
    else	// Otherwise, the process is being started for the first time, so allocate memory and start it
    {
        start_pcb(current_process);
	if (current_process->priority == 0)
	{
	    current_process->mem_block = allocate_memory(reserved_memory, current_process->mbytes);
	}
	else
	{
	    current_process->mem_block = allocate_memory(memory, current_process->mbytes);
	}
    }
}
