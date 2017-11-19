// Matthew Likwarz
// Project 3
// CS 421
// https://stackoverflow.com/questions/36801833/storing-each-line-of-a-text-file-into-an-array

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"

#define CHUNK 1024 /* read 1024 bytes at a time */

enum { MAXL = 40, MAXC = 50 };

void fifo(struct process processes[], int length);

void sjf(struct process processes[], int length);

void roundRobin(struct process processes[], int length, int quantum);

void prettyPrint(struct process processes[], int length);

int main (int argc, char* argv[])
{

	char (*lines)[MAXC] = NULL; /* pointer to array of type char [MAXC] */
    int i, n = 0;
    FILE *fp = argc > 1 ? fopen (argv[1], "r") : stdin;

    if (!fp) {  /* valdiate file open for reading */
        fprintf (stderr, "error: file open failed '%s'.\n", argv[1]);
        return 1;
    }

    if (!(lines = malloc (MAXL * sizeof *lines))) { /* allocate MAXL arrays */
        fprintf (stderr, "error: virtual memory exhausted 'lines'.\n");
        return 1;
    }

    while (n < MAXL && fgets (lines[n], MAXC, fp)) { /* read each line */
        char *p = lines[n];                  /* assign pointer */
        for (; *p && *p != '\n'; p++) {}     /* find 1st '\n'  */
        *p = 0, n++;                         /* nul-termiante  */
    }
    if (fp != stdin) fclose (fp);   /* close file if not stdin */

    /* print lines */
    for (i = 0; i < n; i++) printf (" line[%2d] : '%s'\n", i + 1, lines[i]);
	
	int numProcesses = atoi(lines[0]);
	printf("I hope this works: %d\n", numProcesses);
	
	struct process processes[numProcesses];
	
	// run a parse that will unfuck the data
	for(i=1; i<(numProcesses + 1); i++)
	{
		char *temp = strtok(lines[i], ",");
		processes[i-1].arrival = atoi(temp);
		temp = strtok(NULL, ",");
		processes[i-1].burst = atoi(temp);
		processes[i-1].timeRemaining = atoi(temp);
		processes[i-1].finished = 0;
	}

	
	// grab the round robin quantun
	int quantum = atoi(lines[n-2]);
	printf("Quantum time: %d\n", quantum);
	// grab the MLFQ S value
	int MLFQS = atoi(lines[n-1]);
	printf("MLFQ S: %d\n", MLFQS);

    free (lines);   /* free allocated memory */

	printf("FIFO\n");
	fifo(processes, numProcesses);
	
	printf("SJF\n");
	sjf(processes, numProcesses);

	printf("Round Robin\n");
	roundRobin(processes, numProcesses, quantum);
	

    return 0;
}

void fifo(struct process processes[], int length)
{
	int i;
	int currTime = 0;
	for(i=0; i<length; i++)
	{
		//run the proccess
		currTime += processes[i].burst;
		// update it's finish time
		processes[i].finishTime = currTime;
		// calculate it's turnAround time
		processes[i].turnAroundTime = processes[i].finishTime - processes[i].arrival;
		// calculate it's normalized turnaround time
		processes[i].normalizedTurnAround = processes[i].turnAroundTime / processes[i].burst;
	}
	prettyPrint(processes, length);
}

void sjf(struct process processes[], int length)
{
	printf("Code goes here\n");	
}

void roundRobin(struct process processes[], int length, int quantum)
{
	int i;
	int currTime = 0;
	int totalFinished = 0;
	for(i=0; i<length; i++)
	{
		int remTime = processes[i].timeRemaining;
		if(processes[i].finished == 0)
		{
			if(remTime > quantum)
			{
				// keep er movin
				currTime += quantum;
				processes[i].timeRemaining -= quantum;
			}else
			{
					// ope she done
					currTime += processes[i].timeRemaining;
					processes[i].timeRemaining = 0;
					// update it's finish time
					processes[i].finishTime = currTime;
					// calculate it's turnAround time
					processes[i].turnAroundTime = processes[i].finishTime - processes[i].arrival;
					// calculate it's normalized turnaround time
					processes[i].normalizedTurnAround = processes[i].turnAroundTime / processes[i].burst;
					processes[i].finished = 1;
					totalFinished++;
			}
		}
		
		/*
			if we havent finished all processes and are on the last iteration, then
			flip i back to zero and do it again. Repeat until over.
		*/
		if(totalFinished < length)
		{
			if(i == length-1)
			{
				i=-1;
			}
		}
		
	}
	prettyPrint(processes, length);
}

// This will prettify the printing process
void prettyPrint(struct process processes[], int length)
{
	int i;
	for(i=0; i<length; i++)
	{
		printf("Process %d Arrival: %d\n", i, processes[i].arrival);
		printf("Process %d Burst: %d\n", i, processes[i].burst);
		printf("Process %d Finish: %d\n", i,  processes[i].finishTime);
		printf("Process %d TurnAround: %d\n", i,  processes[i].turnAroundTime);
		printf("Process %d Normalized TurnAround: %.2f\n", i,  processes[i].normalizedTurnAround);
		printf("\n");
	}
}


