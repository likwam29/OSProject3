#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
	// create a file
	FILE *fp;

	// open a write stream
	fp=fopen("inputs.txt","a");

    int numInputs = 10000;
	
	// print out column header to file

	fprintf(fp, "%i\n", numInputs);

	int i;
	for(i=0; i<numInputs; i++)
	{
		int burst = rand() % (20 + 1 - 0) + 0;
		int arrival = rand() % (100 + 1 - 0) + 0;
		fprintf(fp, "%i,%i\n", arrival, burst);
	}
	fprintf(fp, "5");
	fprintf(fp, "10");
	
	// close the file
	fclose(fp);
	return 0;
}

