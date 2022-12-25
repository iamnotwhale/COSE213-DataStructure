#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#include "adt_dlist.h"

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6


// User structure type definition
typedef struct 
{
	char	*name;	// 이름
	int		freq;	// 빈도
} tName;

////////////////////////////////////////////////////////////////////////////////
// Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
//	return	name structure pointer
//			NULL if overflow
tName *createName( char *name, int freq) {
	tName *nName = (tName *) malloc (sizeof(tName));
    nName->name = (char *)malloc(sizeof(char)*100);
	strcpy(nName->name, name);
    nName->freq = freq;
    return nName;
}

// Deletes all data in name structure and recycles memory
void destroyName( void *pName) {
	free(((tName*)pName)->name);
	free(((tName*)pName));
}

////////////////////////////////////////////////////////////////////////////////
// prints contents of name structure
// for traverseList and traverseListR functions
void print_name(const void *dataPtr)
{
	printf( "%s\t%d\n", ((tName *)dataPtr)->name, ((tName *)dataPtr)->freq);
}

////////////////////////////////////////////////////////////////////////////////
// increases freq in name structure
// for addNode function
void increase_freq(const void *dataOutPtr, const void *dataInPtr)
{
	((tName *)dataOutPtr)->freq += ((tName *)dataInPtr)->freq;
}

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'P':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
// compares two names in name structures
// for createList function
int cmpName( const void *pName1, const void *pName2)
{
	return strcmp( ((tName *)pName1)->name, ((tName *)pName2)->name);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	
	char name[100];
	int freq;
	
	tName *pName;
	int ret;
	FILE *fp;
	
	if (argc != 2) {
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty list
	list = createList( cmpName);
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%*d\t%s\t%*c\t%d", name, &freq) != EOF)
	{
		pName = createName( name, freq);
		
		ret = addNode( list, pName, increase_freq);
		
		if (ret == 0 || ret == 2) // failure or duplicated
		{
			destroyName( pName);
		}
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		void *ptr;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list, destroyName);
				return 0;
			
			case FORWARD_PRINT:
				traverseList( list, print_name);
				break;
			
			case BACKWARD_PRINT:
				traverseListR( list, print_name);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a name to find: ");
				fscanf( stdin, "%s", name);
				
				pName = createName( name, 0);

				if (searchList( list, pName, &ptr)) print_name( ptr);
				else fprintf( stdout, "%s not found\n", name);
				
				destroyName( pName);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a name to delete: ");
				fscanf( stdin, "%s", name);
				
				pName = createName( name, 0);

				if (removeNode( list, pName, &ptr))
				{
					fprintf( stdout, "(%s, %d) deleted\n", ((tName *)ptr)->name, ((tName *)ptr)->freq);
					destroyName( (tName *)ptr);
				}
				else fprintf( stdout, "%s not found\n", name);
				
				destroyName( pName);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
		}
		
		if (action) fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
