#include <stdio.h>
#include <string.h> // strdup
#include <stdlib.h>
#include "adt_heap.h"

/* user-defined compare function */
int compare(void *arg1, void *arg2)
{
	return strcmp((char *)arg1, (char *)arg2);
}

/* user-defined print function */
void print_func(void *data)
{
	printf( "%s ", (char *)data);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	HEAP *heap;
	char *newdata;
	char *dataPtr;
	
	char data[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		fprintf( stderr, "file open error: %s\n", argv[1]);
		return 1;
	}
	
	heap = heap_Create( 10, compare); // initial capacity = 10
	
	while (fscanf( fp, "%s", data) != EOF)
	{
		fprintf( stdout, "Inserting %s: ", data);
		
		newdata = strdup(data);
		
		// insert function call
		if (heap_Insert( heap, newdata) == 0) break;
		
		heap_Print( heap, print_func);
 	}
	
	fclose( fp);

	while (!heap_Empty( heap))
	{
		// delete function call
		heap_Delete( heap, (void **)&dataPtr);

		printf( "Deleting  %s: ", dataPtr);

		free(dataPtr);

		heap_Print( heap, print_func);
 	}
	
	heap_Destroy( heap);
	
	return 0;
}
