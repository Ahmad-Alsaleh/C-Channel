#ifndef PQITEM
#define PQITEM
// This item struct is defined to enable assignment of custom priorities
typedef struct{
    int priority;
    int* data;
}PQItem;

#endif