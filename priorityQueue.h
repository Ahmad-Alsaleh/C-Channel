#ifndef PRIORITYQUEUE
#define PRIORITYQUEUE

#include <stddef.h>
#include <time.h>
#include "utils.h"
#include "pqitem.h"
typedef struct{
    PQItem *heap;
    size_t size;
    size_t capacity;
    
}PriorityQueue;

void swap(PQItem *a, PQItem *b);
int create_pq(PriorityQueue*pq, size_t capacity);
int insert(PriorityQueue *pq, int data, int priority);
int extract_max(PriorityQueue *pq);
void pq_destroy(PriorityQueue *pq);


#endif