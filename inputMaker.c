#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

int main(void) {
	// create a file
	FILE *fp;

	// open a write stream
	fp=fopen("inputs.txt","a");

    int numInputs = 10000;
    
    int arrivalArray[numInputs];
	
	// print out column header to file
    int i;
    for(i=0; i<numInputs; i++)
	{
		int arrival = rand() % (100 + 1 - 0) + 0;
		arrivalArray[i] = arrival;
	}
    
    qsort(arrivalArray, numInputs, sizeof(int), compare);

	fprintf(fp, "%i\n", numInputs);

	
	for(i=0; i<numInputs; i++)
	{
		int burst = rand() % (20 + 1 - 1) + 1;
		fprintf(fp, "%i,%i\n", arrivalArray[i], burst);
	}
	fprintf(fp, "5\n");
	fprintf(fp, "10");
	
	// close the file
	fclose(fp);
	return 0;
}

