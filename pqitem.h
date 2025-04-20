#ifndef PQITEM
#define PQITEM
// This item struct is defined to enable assignment of custom priorities
#include<time.h>
typedef struct{
    int priority;
    time_t enqueue_time;
    int data;
}PQItem;

#endif