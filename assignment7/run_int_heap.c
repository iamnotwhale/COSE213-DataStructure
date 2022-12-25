#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#include "adt_heap.h"

#define MAX_ELEM	20

/* user-defined compare function */
int compare(void *arg1, void *arg2)
{
	int *a1 = (int *)arg1;
	int *a2 = (int *)arg2;
	
	return *a1 - *a2;
}

/* user-defined print function */
void print_func(void *data)
{
	printf( " %4d", *(int *)data);
}

int main(void)
{
	HEAP *heap;
	int data;
	int *dataPtr;
	int i;
	
	heap = heap_Create( 10, compare);
	
	srand( time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // 1 ~ MAX_ELEM*3 random number
		
		fprintf( stdout, "Inserting %2d: ", data);
		
		int *newdata = (int *)malloc( sizeof(int));
		*newdata = data;
		
		// insert function call
		heap_Insert( heap, newdata);
		
		heap_Print( heap, print_func);
 	}

	while (!heap_Empty( heap))
	{
		// delete function call
		heap_Delete( heap, (void **)&dataPtr);

		printf( "Deleting  %2d: ", *(int *)dataPtr);

		free(dataPtr);

		heap_Print( heap, print_func);
 	}
	
	heap_Destroy( heap);
	
	return 0;
}

