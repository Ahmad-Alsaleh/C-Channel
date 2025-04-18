#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "channel.h"

// TODO: move this to a different file
// checks if `expression` is true; otherwise, prints an error message to
// `stderr` and exits with code 1
void assert(bool expression, const char *error_msg) {
  if (!expression) {
    fprintf(stderr, "[ERROR] %s\n", error_msg);
    exit(1);
  }
}

// creates a mpmc bounded channel with a specific buffer size
void channel_init(Channel *channel, size_t buffer_size) {
  // TODO: replace this buffer thingy with a ring buffer struct, as well as
  // channel.len.

  // create buffer on the heap and handle error
  channel->buffer = (int *)malloc(buffer_size * sizeof(int));
  assert(channel->buffer != NULL, "`malloc` failed");

  // set the length of the buffer (i.e. the number of items) to 0
  channel->len = 0;

  // create semaphores and handle any errors. but first, we'll unlink existing
  // named semaphores
  sem_unlink("EMPTY");
  channel->empty_slot = sem_open("EMPTY", O_CREAT | O_EXCL, 0666, buffer_size);
  assert(channel->empty_slot != SEM_FAILED,
         "Couldn't create semaphore `EMTPY`");

  sem_unlink("FULL");
  channel->full_slot = sem_open("FULL", O_CREAT | O_EXCL, 0666, 0);
  assert(channel->full_slot != SEM_FAILED, "Couldn't create semaphore `FULL`");

  // initialize buffer mutex
  int error_code = pthread_mutex_init(&channel->buffer_mutex, NULL);
  assert(error_code == 0, "Couldn't initialize buffer mutex");
}

// deallocates a channel freeing all resouces associated to it
void channel_destroy(Channel *channel) {
  // deallocat the heap
  free(channel->buffer);

  // close and unlink semaphores and handle any errors
  int error_code;
  error_code = sem_close(channel->empty_slot);
  assert(error_code == 0, "Couldn't close semaphore `EMPTY`");

  error_code = sem_close(channel->full_slot);
  assert(error_code == 0, "Couldn't close semaphore `FULL`");

  error_code = sem_unlink("EMPTY");
  assert(error_code == 0, "Couldn't unlink semaphore `EMPTY`");

  error_code = sem_unlink("FULL");
  assert(error_code == 0, "Couldn't unlink semaphore `FULL`");

  // free the resouces of the mutex
  error_code = pthread_mutex_destroy(&channel->buffer_mutex);
  assert(error_code == 0,
         "Couldn't distroy buffer mutex. Are you sure the mutex is unlocked?");
}

// produces an item
void channel_send(Channel *channel, int value) {
  int error_code;

  error_code = sem_wait(channel->empty_slot);
  assert(error_code == 0, "Couldn't wait on semaphore `EMPTY`");

  error_code = pthread_mutex_lock(&channel->buffer_mutex);
  assert(error_code == 0, "Couldn't lock buffer mutex");

  // TODO: change this to buffer.push once the deque is implamented
  channel->buffer[channel->len] = value;
  channel->len += 1;

  error_code = pthread_mutex_unlock(&channel->buffer_mutex);
  assert(error_code == 0, "Couldn't unlock buffer mutex");

  error_code = sem_post(channel->full_slot);
  assert(error_code == 0, "Couldn't signal semaphore `FULL`. Are you sure the "
                          "semaphore descriptor is still valid?");
}

// consumes an item
int channel_recv() {
  // TOOD
  return 0;
}
