#include <stdio.h>
#include <stdlib.h> // malloc, realloc

#include "adt_heap.h"

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index);


/* Reestablishes heap by moving data in index down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index);

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2));

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap);

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr);

/* Deletes index of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr);

/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap);

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data));

/////////////////////////////////////////////////////////////////
/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index) {
    int parent;
    void* hold;

        if (index) {
            parent = (index - 1)/ 2;
            if (heap->compare(heap->heapArr[index], heap->heapArr[parent]) > 0) {
                hold = heap->heapArr[parent];
                heap->heapArr[parent] = heap->heapArr[index];
                heap->heapArr[index] = hold;
                _reheapUp (heap, parent);
            }
        }
}


/* Reestablishes heap by moving data in index down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index) {
    void* hold;
    void* leftKey;
    void* rightKey;
    int largeSubtree;

    if ((index * 2 + 1) <= heap->last) {
        leftKey = heap->heapArr[index * 2 + 1];
        if ((index * 2 + 2) <= heap->last) rightKey = heap->heapArr[index * 2 + 2];
        else rightKey = NULL;

        if ((!rightKey) || heap->compare(leftKey, rightKey) >= 0) {
            largeSubtree = index * 2 + 1;
        }
        else {
            largeSubtree = index * 2 + 2;
        }
        if (heap->compare(heap->heapArr[index], heap->heapArr[largeSubtree]) < 0) {
            hold = heap->heapArr[index];
            heap->heapArr[index] = heap->heapArr[largeSubtree];
            heap->heapArr[largeSubtree] = hold;
            _reheapDown (heap, largeSubtree);
        }
    }
}

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2)) {
    HEAP *newHeap = (HEAP *) malloc (sizeof(HEAP));
    
    newHeap->last = -1;
    newHeap->capacity = capacity;
    newHeap->compare = compare;

    newHeap->heapArr = (void **) malloc(sizeof(void*) * capacity);
    return newHeap;
}

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap) {
    void *cur;
    while (!heap_Empty(heap)) {
        cur = heap->heapArr[heap->last];
        heap->last--;
        free(cur);
    }
    free(heap->heapArr);
    free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr) {
    if (heap_Empty(heap)) {
        heap->last = 0;
        heap->heapArr[heap->last] = dataPtr;
    }
    else {
        if (heap->capacity == heap->last + 1) {
            heap->capacity *= 2;
            heap->heapArr = (void**) realloc(heap->heapArr, heap->capacity * sizeof(void*));
            heap->last++;
            heap->heapArr[heap->last] = dataPtr;
        }
        else {
            heap->last++;
            heap->heapArr[heap->last] = dataPtr;
        }
        _reheapUp(heap, heap->last);
    }
    return 1;
}

/* Deletes index of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr) {
    if (heap_Empty(heap)) return 0;
    *dataOutPtr = heap->heapArr[0];
    heap->heapArr[0] = heap->heapArr[heap->last];
    heap->last--;
    _reheapDown(heap, 0);
    return 1;
}

/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap) {
    return (heap->last == -1) ? 1: 0;
}

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data)) {
    for (int i=0; i<=heap->last; i++) {
       print_func(heap->heapArr[i]);
    }
    printf("\n");
}