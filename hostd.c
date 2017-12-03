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
        printf("Error opening file");
        exit(0);
    }

    fill_input_queue(input_file, input_list_stream, input_queue);

    initialize_system(memory, rsrcs);
    
    while (!complete(input_queue, real_time_queue, user_job_queue, priority_one_queue, priority_two_queue, priority_three_queue, current_process))
    {
        printf("Checkpoint 1...\n");

        while (input_queue && input_queue->arrival_time <= timer)
        {
            printf("Conditional 1.\n");

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

        printf("Checkpoint 2...\n");

        while(user_job_queue && fit_memory(memory, user_job_queue->mbytes) && 
        check_resources(user_job_queue, rsrcs))
        {
            printf("Conditional 2.\n");

            struct pcb* p = dequeue_pcb(&user_job_queue);
            struct mab* m = allocate_memory(memory, p->mbytes);
            allocate_resources(p, rsrcs);
            switch(p->priority)
            {
                case 1:
                    enqueue_pcb(priority_one_queue, p);
                    break;
                case 2:
                    enqueue_pcb(priority_two_queue, p);
                    break;
                default:
                    enqueue_pcb(priority_three_queue, p);
                    break;
            }
        }
        
        printf("Checkpoint 3...\n");

        if (current_process)    // There is a process currently running
        {
            printf("Conditional 3.\n");
            if (--current_process->remaining_cpu_time == 0) // The process has finished
            {
                terminate_pcb(current_process);
                free_memory(current_process->mem_block);
                free_resources(current_process, rsrcs);
                free(current_process);
                current_process = NULL;
            }
            else    // The process hasn't finished
            {   
                if (real_time_queue || user_job_queue || priority_one_queue || priority_two_queue || priority_three_queue &&    
                current_process->priority != 0)     // There are still jobs in the queue(s)
                {
                    struct pcb* p = suspend_pcb(current_process);
                    if (++p->priority > 3)
                    {
                        p->priority = 3;
                    }
                    switch(p->priority)
                    {
                        case 1:
                            enqueue_pcb(priority_one_queue, p);
                            break;
                        case 2:
                            enqueue_pcb(priority_two_queue, p);
                            break;
                        default:
                            enqueue_pcb(priority_three_queue, p);
                            break;
                    }
                    current_process = NULL;
                }
            }
        }

        printf("Checkpoint 4...\n");

        if (real_time_queue || priority_one_queue || priority_two_queue || priority_three_queue)
        {
            printf("Conditional 4.\n");

            if (real_time_queue)
            {
                current_process = dequeue_pcb(&real_time_queue);
            }
            else if (priority_one_queue)
            {
                current_process = dequeue_pcb(&priority_one_queue);
            }
            else if (priority_two_queue)
            {
                current_process = dequeue_pcb(&priority_two_queue);
            }
            else
            {
                current_process = dequeue_pcb(&priority_three_queue);
            }

            if (current_process->pid != 0)
            {
                restart_pcb(current_process);
            }
            else
            {
                start_pcb(current_process);
            }
        }

        printf("Checkpoint 5...\n");

        printf("Time remaining on current process: %d", current_process->remaining_cpu_time);

        sleep(1);
        timer += 1;
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
    char line[50];

    while (fgets(line, 50, input_list_stream))
    {
        process = create_null_pcb();

        char* s = strtok(line, ",");
        int nums[8];
        int i = 0;

        while (s)
        {
            int n = atoi(s);
            s = strtok(NULL, ",");
            nums[i] = n;
            ++i;
        }

        process->arrival_time = nums[0];
        process->priority = nums[1];
        process->remaining_cpu_time = nums[2];
        process->mbytes = nums[3];
        process->num_printers = nums[4];
        process->num_scanners = nums[5];
        process->num_modems = nums[6];
        process->num_drives = nums[7];

        enqueue_pcb(input_queue, process);
        process = NULL;
    }
    fclose(input_list_stream);
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