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

    struct mab* memory = NULL;

    struct resources* rsrcs = NULL;

    unsigned int timer = 0;

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

    initialize_system(memory, rsrcs);
    
    while (!complete(input_queue, real_time_queue, user_job_queue, priority_one_queue, priority_two_queue, priority_three_queue, current_process))
    {
        while (input_queue && input_queue->arrival_time <= timer)
        {
            struct pcb* p = dequeue_pcb(&input_queue);
            if (p->priority == 0)
            {
                real_time_queue = enqueue_pcb(real_time_queue, p);
            }
            else
            {
                user_job_queue = enqueue_pcb(user_job_queue, p);
            }
        }

        while(user_job_queue && memory_fit(memory, user_job_queue->mbytes) && 
        check_resources(user_job_queue, rsrcs))
        {
            struct pcb* p = dequeue_pcb(&user_job_queue);
            struct mab* m = allocate_memory(memory, p->mbytes);
        }
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
        if (fscanf(input_list_stream, "%d, %d, %d, %d, %d, %d, %d, %d",
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

void initialize_system(struct mab* mem, struct resources* rsrcs)
{
    mem = create_null_mab();
    mem->size = TOTAL_MEMORY;
    rsrcs = create_null_resources();
    rsrcs->remaining_printers = TOTAL_PRINTERS;
    rsrcs->remaining_scanners = TOTAL_SCANNERS;
    rsrcs->remaining_modems = TOTAL_MODEMS;
    rsrcs->remaining_drives = TOTAL_DRIVES;
}

bool complete(struct pcb* i_q, struct pcb* r_t_q, struct pcb* u_j_q, struct pcb* p_one_q, struct pcb* p_two_q, struct pcb* p_three_q, struct pcb* cur_p)
{
    return (i_q || r_t_q || u_j_q || p_one_q || p_two_q || p_three_q || cur_p);
}