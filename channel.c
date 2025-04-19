#include <pthread.h>
#include <stdio.h>

#include "channel.h"
#include "deque.h"
#include "utils.h"

// initializes a mpmc bounded channel with a specific buffer size
void channel_init(Channel *channel, size_t buffer_size) {
  int error_code;

  // create a FIFO queue
  error_code = deque_init(&channel->buffer, buffer_size);
  assert_value(error_code == 0, "Couldn't create the buffer of the channel");

  // create semaphores and handle any errors. but first, we'll unlink existing
  // named semaphores
  sem_unlink("EMPTY");
  channel->empty_slot = sem_open("EMPTY", O_CREAT | O_EXCL, 0666, buffer_size);
  assert_value(channel->empty_slot != SEM_FAILED,
               "Couldn't create semaphore `EMTPY`");

  sem_unlink("FULL");
  channel->full_slot = sem_open("FULL", O_CREAT | O_EXCL, 0666, 0);
  assert_value(channel->full_slot != SEM_FAILED,
               "Couldn't create semaphore `FULL`");

  // initialize buffer mutex using default attributes
  error_code = pthread_mutex_init(&channel->buffer_mutex, NULL);
  assert_value(error_code == 0, "Couldn't initialize buffer mutex");
}

// deallocates a channel freeing all resources associated to it
void channel_destroy(Channel *channel) {
  // deallocate the FIFO buffer
  deque_destroy(&channel->buffer);

  // close and unlink semaphores and handle any errors
  int error_code;
  error_code = sem_close(channel->empty_slot);
  assert_value(error_code == 0, "Couldn't close semaphore `EMPTY`");

  error_code = sem_close(channel->full_slot);
  assert_value(error_code == 0, "Couldn't close semaphore `FULL`");

  error_code = sem_unlink("EMPTY");
  assert_value(error_code == 0, "Couldn't unlink semaphore `EMPTY`");

  error_code = sem_unlink("FULL");
  assert_value(error_code == 0, "Couldn't unlink semaphore `FULL`");

  // free the resources of the mutex
  error_code = pthread_mutex_destroy(&channel->buffer_mutex);
  assert_value(
      error_code == 0,
      "Couldn't destroy buffer mutex. Are you sure the mutex is unlocked?");
}

// produces an item. if `log` is true, the consumed item will be printed
void channel_send(Channel *channel, int value, bool log) {
  int error_code;

  // wait for a free slot in the buffer to become available to be able to insert
  // the new item to the buffer
  error_code = sem_wait(channel->empty_slot);
  assert_value(error_code == 0, "Couldn't wait on semaphore `EMPTY`");

  // once a free slot becomes available in the buffer, require the buffer lock
  // to manipulate the buffer without race conditions
  error_code = pthread_mutex_lock(&channel->buffer_mutex);
  assert_value(error_code == 0, "Couldn't lock buffer mutex");

  // push the new item to the end of the FIFO queue
  deque_push(&channel->buffer, value);

  // release the buffer lock to allow other threads to acquire the lock
  error_code = pthread_mutex_unlock(&channel->buffer_mutex);
  assert_value(error_code == 0, "Couldn't unlock buffer mutex");

  if (log)
    printf("[Producer] sent %d\n", value);

  // signal to other threads waiting to consume an item that a new item is
  // available in the buffer
  error_code = sem_post(channel->full_slot);
  assert_value(error_code == 0,
               "Couldn't signal semaphore `FULL`. Are you sure the "
               "semaphore descriptor is still valid?");
}

// consumes an item and returns it. if `log` is true, the consumed item will be
// printed
int channel_recv(Channel *channel, bool log) {
  int error_code;

  // wait for an item becomes available in the buffer
  error_code = sem_wait(channel->full_slot);
  assert_value(error_code == 0, "Couldn't wait on semaphore `FULL`");

  // once an item becomes available in the buffer, require the buffer lock to
  // manipulate the buffer without race conditions
  error_code = pthread_mutex_lock(&channel->buffer_mutex);
  assert_value(error_code == 0, "Couldn't lock buffer mutex");

  // pop an item from the beginning of the FIFO queue
  int item = deque_pop(&channel->buffer);

  // release the buffer lock to allow other threads to acquire the lock
  error_code = pthread_mutex_unlock(&channel->buffer_mutex);
  assert_value(error_code == 0, "Couldn't unlock buffer mutex");

  if (log)
    printf("[Consumer] received %d\n", item);

  // signal to other threads waiting to produce a new item that a free slot slot
  // is available in the buffer
  error_code = sem_post(channel->empty_slot);
  assert_value(error_code == 0,
               "Couldn't signal semaphore `EMPTY`. Are you sure the "
               "semaphore descriptor is still valid?");

  return item;
}
