#ifndef CHANNEL_H
#define CHANNEL_H

#include <semaphore.h>
#include <stddef.h>

#include "deque.h"

// a many-producer-many-consumer (a.k.a mpmc) bounded channel which is an
// implementation of the producer-consumer problem.
typedef struct {
  Deque buffer;
  sem_t *empty_slot;
  sem_t *full_slot;
  pthread_mutex_t buffer_mutex;
} Channel;

void channel_init(Channel *channel, size_t buffer_size);
void channel_destroy(Channel *channel);
void channel_send(Channel *channel, int value, const char *log_msg);
int channel_recv(Channel *channel, const char *log_msg);

#endif
