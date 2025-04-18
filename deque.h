#ifndef DEQUE
#define DEQUE

#include <stddef.h>

#include "utils.h"

// a double-ended queue (deque) which is a FIFO queue.
// this data structure is also known as a ring queue.
typedef struct {
  int *buffer;
  size_t len;
  size_t capacity;
  size_t first_i;
  size_t last_i;
} Deque;

int deque_init(Deque *deque, size_t size);
void deque_destroy(Deque *deque);
void deque_push(Deque *deque, int item);
int deque_pop(Deque *deque);

#endif
