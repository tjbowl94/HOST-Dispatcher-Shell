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
    struct_pcb* current_process = NULL;
    struct_pcb* process = NULL;
    int timer = 0;

    if (argc == 2)
    {
        input_file = argv[1];
    }
    else
    {
        print_usage()
    }

    return 0;
}

void print_usage()
{
    printf("Please supply a file specifying a sequence of processes. as an argument when running this file.");
}