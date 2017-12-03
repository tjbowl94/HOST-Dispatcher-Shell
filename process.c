/*
**
**      author:     Diego Asis, Jeremiah Bolin, Thomas Bowlin
**      date:       11/28/2017
**      class:      CSC400, Fall 2017 - Dr. Zhang
**
**      file description: 
**
*/


#include "process.h"


struct pcb* create_null_pcb()
{
    struct pcb* p = (struct pcb*)malloc(sizeof(struct pcb));
    p->pid = 0;
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

struct pcb* start_pcb(struct pcb* p)
{
    if (p->pid == 0)
    {
        switch(p->pid = fork())
        {
            case -1:
                // Handle error
                exit(1);
            case 0:
                p->pid = getpid();
                p->status = PCB_RUNNING;
                // Handle printing running status
                fflush(stdout);
		execvp(p->args[0], p->args);
                exit(2);
        }
    }
    else
    {
        kill(p->pid, SIGCONT);
    }
    p->status = PCB_RUNNING;
    return p;
}

struct pcb* suspend_pcb(struct pcb* p)
{
    int status;
    kill(p->pid, SIGTSTP);
    waitpid(p->pid, &status, WUNTRACED);
    p->status = PCB_SUSPENDED;
    return p;
}

struct pcb* restart_pcb(struct pcb* p)
{
    kill(p->pid, SIGCONT);
    return p;
}

struct pcb* terminate_pcb(struct pcb* p)
{
    int status;
    kill(p->pid, SIGINT);
    waitpid(p->pid, &status, WUNTRACED);
    p->status = PCB_TERMINATED;
    return p;
}

struct pcb* enqueue_pcb(struct pcb* head, struct pcb* p)
{
    struct pcb* curPcb = head;
    if (head)
    {
        while (curPcb->next)
        {
            curPcb = curPcb->next;
        }
        curPcb->next = p;
        return head;
    }
    else
    {
        return p;
    }
}

struct pcb* dequeue_pcb(struct pcb** head)
{
    struct pcb* p;
    if (head && (p = *head))
    {
        *head = p->next;
        return p;
    }
    else
    {
        return NULL;
    }
}
