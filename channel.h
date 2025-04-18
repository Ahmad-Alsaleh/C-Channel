#ifndef CHANNEL_H
#define CHANNEL_H

#include <semaphore.h>
#include <stddef.h>

// a  many-producer-many-consumer (a.k.a mpmc) bounded channel which is an
// implamentation of the producer-consumer problem.
typedef struct {
  int *buffer;
  size_t len;
  sem_t *empty_slot;
  sem_t *full_slot;
  pthread_mutex_t buffer_mutex;
} Channel;

// creates a channel
void channel_init(Channel *channel, size_t buffer_size);

// deallocates a channel
void channel_destroy(Channel *channel);

// produces an item
void channel_send(Channel *channel, int value);

// consumes an item
int channel_recv(Channel *channel);

#endif
