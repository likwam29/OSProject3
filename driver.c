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

		printf("arrival %d\n", processes[i-1].arrival);
		printf("burst %d\n", processes[i-1].burst);
	}

	
	// grab the round robin quantun

	// grab the MLFQ S value

    free (lines);   /* free allocated memory */

    return 0;
}

