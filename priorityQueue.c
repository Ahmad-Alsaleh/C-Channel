#include <stddef.h>
#include <stdlib.h>

#include "priorityQueue.h"
#include "utils.h"
#define AGING_FACTOR 0.5
static long logical_time = 0; // updates each insert
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
    pq->heap[i].enqueue_time = logical_time++;

    while(i > 0 && effective_priority(&pq->heap[i]) > effective_priority(&pq->heap[(i - 1) /2])){
        swap(&pq->heap[i], &pq->heap[(i-1)/2]);
        i = (i-1)/2;
    }
    // Success
    return 0;

}

double effective_priority(PQItem *item){
    return item->priority + AGING_FACTOR * (logical_time - item->enqueue_time);
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
        if (left < pq->size && effective_priority(&pq->heap[left]) > effective_priority(&pq->heap[largest]))
            largest = left;
        if (right < pq->size && effective_priority(&pq->heap[right]) > effective_priority(&pq->heap[largest]))
            largest = right;
        if (largest == i)
            break;
        swap(&pq->heap[i], &pq->heap[largest]);
        i = largest;
        
    }
    return out;
}
int effective_priority(PQItem *item) {
    return item->priority + AGING_FACTOR * (logical_time - item->enqueue_time);
}

void destroy(PriorityQueue *pq){
    free(pq->heap);
}
