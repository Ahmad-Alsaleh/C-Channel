#include <stddef.h>
#include <stdlib.h>

#include "priorityQueue.h"
#include "utils.h"

// Swap two elements
void swap(PQItem *a, PQItem *b){
    PQItem temp = *a;
    *a = *b;
    *b = temp;
}

// Create a priority queue with the given capacity
int create_pq(PriorityQueue *pq,size_t capacity){
    pq->heap = malloc(sizeof(PQItem) * capacity);
    if (pq->heap == NULL)
        return -1;
    pq->size = 0; //initialize to empty
    pq->capacity = capacity;
    return 0;
}
int insert(PriorityQueue *pq, int *data, int priority){
    assert_value(pq->size < pq->capacity, "Trying to insert into a full heap");
    int i = pq->size++;
    pq->heap[i].priority = priority;
    pq->heap[i].data = data;

    while(i > 0 && pq->heap[i].priority > pq->heap[(i - 1) /2].priority){
        swap(&pq->heap[i], &pq->heap[(i-1)/2]);
        i = (i-1)/2;
    }
    // Success
    return 0;

}

int *extract_max(PriorityQueue *pq){
    assert_value(pq->size>0, "Trying to extract from an empty heap");

    int *out = pq->heap[0].data;
    pq->heap[0] = pq->heap[--pq->size];

    int i = 0;

    // heapify after extracting
    while(1){
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int largest = i;
        if (left < pq->size && pq->heap[left].priority > pq->heap[largest].priority)
            largest = left;
        if (right < pq->size && pq->heap[right].priority > pq->heap[largest].priority)
            largest = right;
        if (largest == i)
            break;
        swap(&pq->heap[i], &pq->heap[largest]);
        i = largest;
        
    }
    return out;
}


void destroy(PriorityQueue *pq){
    free(pq->heap);
}
