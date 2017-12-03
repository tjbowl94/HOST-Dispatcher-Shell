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

/*
**  Global Variables
*/
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

int main(int argc, char* argv[])
{
    char* input_file;
    FILE* input_list_stream;

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

    fill_input_queue(input_file, input_list_stream);

    if (input_queue)
    {
        printf("Input queue not null..2\n");
    }

    initialize_system(memory, rsrcs);

    if (!memory)
    {
	printf("No memory\n");
    }
    
    while (!complete())
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
                printf("a.1\n");
                terminate_pcb(current_process);
                printf("a.2\n");
                free_memory(current_process->mem_block);
                printf("a.3\n");
                free_resources(current_process, rsrcs);
                printf("a.4\n");
                free(current_process);
                printf("a.5\n");
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
		current_process->mem_block = allocate_memory(memory, current_process->mbytes);
            }
        }

        printf("Checkpoint 5...\n");

	if (current_process)
	{
	    printf("Time remaining on current process: %d\n", current_process->remaining_cpu_time);
	}

        sleep(1);
        timer += 1;
    }

    printf("All processes have finished.\n");

    return 0;
}

void print_usage()
{
    printf("\nPlease supply a file specifying a sequence of processes as an argument when running this file.\n\n");
}

void fill_input_queue(char* input_file, FILE* input_list_stream)
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

        input_queue = enqueue_pcb(input_queue, process);

        if (input_queue)
        {
            printf("Input queue not null\n");
        }

        process = NULL;
    }

    fclose(input_list_stream);
}

void initialize_system()
{
    memory = create_null_mab();
    memory->size = TOTAL_MEMORY;
    rsrcs = create_null_resources();
    rsrcs->remaining_printers = TOTAL_PRINTERS;
    rsrcs->remaining_scanners = TOTAL_SCANNERS;
    rsrcs->remaining_modems = TOTAL_MODEMS;
    rsrcs->remaining_drives = TOTAL_DRIVES;
}

bool complete()
{
    return (!input_queue && !real_time_queue && !user_job_queue && !priority_one_queue 
        && !priority_two_queue && !priority_three_queue && !current_process);
}
