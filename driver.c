// Matthew Likwarz
// Project 3
// CS 421
// figuring out how to read in a file and all that good jazz was brought to you be the guy/girl from the link below
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

void mlfq(struct process processes[], int length, int s);

void prettyPrint(struct process processes[], int length);

void createCSV(char *name, struct process processes[], int length);

void resetProcessObject(struct process processes[], int length);

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
    if (fp != stdin)
	{
		fclose (fp);   /* close file if not stdin */
		printf("first file closed\n");
	}

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
	createCSV("fifo.csv", processes, numProcesses);
	resetProcessObject(processes, numProcesses);
	
	printf("SJF\n");
	sjf(processes, numProcesses);
	createCSV("sjf.csv", processes, numProcesses);
	resetProcessObject(processes, numProcesses);

	printf("Round Robin\n");
	roundRobin(processes, numProcesses, quantum);
	createCSV("roundRobin.csv", processes, numProcesses);
	resetProcessObject(processes, numProcesses);

	printf("MLFQ\n");
	mlfq(processes, numProcesses, MLFQS);
	//createCSV("mlfq.csv", processes, numProcesses);

    return 0;
}

// This method will simulate a fifo scheduler
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
		processes[i].normalizedTurnAround = (float) processes[i].turnAroundTime / (float) processes[i].burst;
		processes[i].latency = processes[i].turnAroundTime - processes[i].burst;
	}
	prettyPrint(processes, length);
}

// This method will simulate shortest job first scheduling
void sjf(struct process processes[], int length)
{
	int totalFinished = 0;
	int currTime = 0;
	while(totalFinished != length)
	{
		// increment currTime by 1
		currTime++;
		
		// check for the index of the lowest remainingTime where arrival <= currTime + !finished
		int i;
		int tempIndex = 0;
		int minVal = INT_MAX;
		for(i=0; i<length; i++)
		{
			if(processes[i].finished == 0 && processes[i].arrival <= currTime)
			{
				if(processes[i].timeRemaining < minVal)
				{
					minVal = processes[i].timeRemaining;
					tempIndex = i;
				}
			}
		}
		// subtract 1 from remaining time of shortest one
		processes[tempIndex].timeRemaining--;

		// if remainingTime == 0 then do finish up process increment total, mark finished

		if(processes[tempIndex].timeRemaining == 0)
		{
			processes[tempIndex].finishTime = currTime;
			// calculate it's turnAround time
			processes[tempIndex].turnAroundTime = processes[tempIndex].finishTime - processes[tempIndex].arrival;
			// calculate it's normalized turnaround time
			processes[tempIndex].normalizedTurnAround = (float) processes[tempIndex].turnAroundTime / (float) processes[tempIndex].burst;

			processes[tempIndex].latency = processes[tempIndex].turnAroundTime - processes[tempIndex].burst;
			processes[tempIndex].finished = 1;
			totalFinished++;
		}
	}	
	prettyPrint(processes, length);
}

// This method will simulate a round robin scheduler
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
				// keep'er movin
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
					processes[i].normalizedTurnAround = (float) processes[i].turnAroundTime / (float) processes[i].burst;

					processes[i].latency = processes[i].turnAroundTime - processes[i].burst;
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
				// assign to -1 because after this line the for loop will increment
				i=-1;
			}
		}
		
	}
	prettyPrint(processes, length);
}

void mlfq(struct process processes[], int length, int s)
{
	printf("Code goes here");
}

// This will prettify the printing process
void prettyPrint(struct process processes[], int length)
{
	int i;
	float avgLatency = 0;
	float avgTurnaround = 0;
	float avgNormalizedTurn = 0;
	
	for(i=0; i<length; i++)
	{
		avgLatency += (float) processes[i].latency;
		avgTurnaround += (float) processes[i].turnAroundTime;
		avgNormalizedTurn += (float) processes[i].normalizedTurnAround;
		
		printf("Process %d Arrival: %d\n", i, processes[i].arrival);
		printf("Process %d Burst: %d\n", i, processes[i].burst);
		printf("Process %d Finish: %d\n", i,  processes[i].finishTime);
		printf("Process %d TurnAround: %d\n", i,  processes[i].turnAroundTime);
		printf("Process %d Normalized TurnAround: %.2f\n", i,  processes[i].normalizedTurnAround);
		printf("\n");
	}
	
	avgLatency =  avgLatency / (float) length;
	avgTurnaround = avgTurnaround / (float) length;
	avgNormalizedTurn = avgNormalizedTurn / (float) length;

	
	printf("Average Latency: %.2f\n", avgLatency);
	printf("Average Turnaround: %.2f\n", avgTurnaround);
	printf("Average Normalized Turnaround: %.2f\n", avgNormalizedTurn);
	
}

// this will create a .csv file for the specific scheduling algorithm
void createCSV(char *name, struct process processes[], int length)
{
	// create a file
	FILE *fp;

	// open a write stream
	fp=fopen(name,"w+");

	// Write into csv
	
	// print out column header to file

	fprintf(fp, "Process number,finish time, turnaround time,normalized turnaround time");

	int i;
	float avgLatency = 0;
	float avgTurnaround = 0;
	float avgNormalizedTurn = 0;
	for(i=0; i<length -1; i++)
	{
		avgLatency += (float) processes[i].latency;
		avgTurnaround += (float) processes[i].turnAroundTime;
		avgNormalizedTurn += (float) processes[i].normalizedTurnAround;
		
		fprintf(fp, "\nProcess %i,", i);
		fprintf(fp, "%i,", processes[i].finishTime);
		fprintf(fp, "%i,", processes[i].turnAroundTime);
		fprintf(fp, "%.2f", processes[i].normalizedTurnAround);
	}

	avgLatency += (float) processes[length-1].latency;
	avgTurnaround += (float) processes[length-1].turnAroundTime;
	avgNormalizedTurn += (float) processes[length-1].normalizedTurnAround;

	avgLatency =  avgLatency / (float) length;
	avgTurnaround = avgTurnaround / (float) length;
	avgNormalizedTurn = avgNormalizedTurn / (float) length;
	
	// this will print the last process without a comma after it.
	fprintf(fp, "\nProcess %i,", length-1);
	fprintf(fp, "%i,", processes[length-1].finishTime);
	fprintf(fp, "%i,", processes[length-1].turnAroundTime);
	fprintf(fp, "%.2f", processes[length-1].normalizedTurnAround);

	fprintf(fp, "\n\nAverage Latency:, %.2f", avgLatency);
	fprintf(fp, "\n\nAverage Turnaround:, %.2f", avgTurnaround);
	fprintf(fp, "\n\nAverage Normalized Turnaround:, %.2f", avgNormalizedTurn);
	
	// close the file
	fclose(fp);
}

/* 
	This will reset the processes object to the same
	state it was in when it was first created
*/ 
void resetProcessObject(struct process processes[], int length)
{
	int i;
	for(i=0; i<length; i++)
	{
		processes[i].finishTime = 0;
		processes[i].turnAroundTime = 0;
		processes[i].normalizedTurnAround = 0;
		processes[i].timeRemaining = processes[i].burst;
		processes[i].finished = 0;
		processes[i].latency = 0;
	}
}


