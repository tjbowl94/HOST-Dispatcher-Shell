/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include "hostd.h"


int main(int argc, char* argv[])
{
    char* input_file;
    FILE* input_list_stream;

    struct pcb* input_queue = NULL;
    struct pcb* real_time_queue = NULL;
    struct pcb* user_job_queue = NULL;
    struct pcb* priority_one_queue = NULL;
    struct pcb* priority_two_queue = NULL;
    struct pcb* priority_three_queue = NULL;

    struct pcb* current_process = NULL;
    struct pcb* process = NULL;

    int timer = 0;

    if (argc == 2)
    {
        input_file = argv[1];
    }
    else
    {
        print_usage();
        exit(0);
    }

    if (!(input_list_stream = fopen(input_file, "r")))
    {
        // Print error message
        exit(0);
    }

    fill_input_queue(input_file, input_list_stream, input_queue);
    
    while (input_queue || current_process)
    {

    }

    return 0;
}

void print_usage()
{
    printf("\nPlease supply a file specifying a sequence of processes as an argument when running this file.\n\n");
}

void fill_input_queue(char* input_file, FILE* input_list_stream, struct pcb* input_queue)
{
    struct pcb* process;

    while (!feof(input_list_stream))
    {
        process = create_null_pcb();
        if (fscanf(input_list_stream, "%d", "%d", "%d", "%d", "%d", "%d", "%d", "%d"
            &(process->arrival_time),
            &(process->priority),
            &(process->remaining_cpu_time),
            &(process->mbytes),
            &(process->num_printers),
            &(process->num_scanners),
            &(process->num_modems),
            &(process->num_drives)) != 8)
        {
            free(process);
            continue;
        }
        process->status = PCB_INITIALIZED;
        input_queue = enqueue_pcb(input_queue, process);
    }
}