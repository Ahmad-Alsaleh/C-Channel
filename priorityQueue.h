#ifndef PRIORITYQUEUE
#define PRIORITYQUEUE

#include <stddef.h>
#include <time.h>

#include "utils.h"

typedef struct {
  int priority;
  time_t enqueue_time;
  int data;
} PQItem;

typedef struct {
  PQItem *heap;
  size_t size;
  size_t capacity;
} PriorityQueue;

void pqitem_swap(PQItem *a, PQItem *b);
int pq_create(PriorityQueue *pq, size_t capacity);
int pq_insert(PriorityQueue *pq, int data, int priority);
int pq_extract_max(PriorityQueue *pq);
void pq_destroy(PriorityQueue *pq);

#endif
