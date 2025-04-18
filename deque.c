#include <stddef.h>
#include <stdlib.h>

#include "deque.h"
#include "utils.h"

// creates a double-ended queue (deque).
// this function returns 0 if succeeds, -1 otherwise
int deque_init(Deque *deque, size_t size) {
  deque->buffer = (int *)malloc(size * sizeof(int));
  if (deque->buffer == NULL)
    return -1;

  deque->len = 0;
  deque->capacity = size;
  deque->first_i = 0;
  deque->last_i = 0;

  return 0;
}

// deallocates a dequeue freeing all resources associated to it
void deque_destroy(Deque *deque) { free(deque->buffer); }

// pushed an item to the end of the deque
void deque_push(Deque *deque, int item) {
  assert_value(deque->len < deque->capacity,
               "Trying to push into a full deque");

  deque->buffer[deque->last_i] = item;

  if (deque->last_i + 1 == deque->capacity)
    deque->last_i = 0;
  else
    deque->last_i += 1;

  deque->len += 1;
}

// pops an item from the beginning of the queue and returns it
int deque_pop(Deque *deque) {
  assert_value(deque->len > 0, "Trying to pop from an empty deque");

  int item = deque->buffer[deque->first_i];

  if (deque->first_i + 1 == deque->capacity)
    deque->first_i = 0;
  else
    deque->first_i += 1;

  deque->len -= 1;

  return item;
};
