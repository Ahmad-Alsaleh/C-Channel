#ifndef PQITEM
#define PQITEM
// This item struct is defined to enable assignment of custom priorities
typedef struct{
    int priority;
    long enqueue_time;
    int* data;
}PQItem;

#endif