#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "channel.h"
#include "configs.h"
#include "utils.h"

typedef struct {
  Channel *channel;
  int id;
  int n_items_to_consume;
  int priority;
} ThreadArg;

void *producer_routine(void *arg) {
  ThreadArg *thread_arg = (ThreadArg *)arg;

  if (MODE == Priority) {
    thread_arg->priority = rand() % 5 + 1; // pseudo-random from 1 to 5
    printf("Producer #%d has priority %d.\n", thread_arg->id,
           thread_arg->priority);
  }

  int first_item = NUM_OF_ITEMS_TO_PRODUCE_PER_THREAD * thread_arg->id;
  int last_item = first_item + NUM_OF_ITEMS_TO_PRODUCE_PER_THREAD - 1;

  for (int item = first_item; item <= last_item; ++item) {
    // sleep pretend some work is being done to create the new item
    if (thread_arg->id % 2 == 0)
      sleep(rand() % 3 + 1);

    if (MODE == Fifo)
      channel_send(thread_arg->channel, item, thread_arg->id);
    else
      pq_channel_send(thread_arg->channel, item, thread_arg->id,
                      thread_arg->priority);
  }

  printf("Producer #%d finished. Items sent: all numbers between %d and %d\n",
         thread_arg->id, first_item, last_item);

  return NULL;
}

void *consumer_routine(void *arg) {
  ThreadArg *thread_arg = (ThreadArg *)arg;

  int *received_items =
      (int *)malloc(thread_arg->n_items_to_consume * sizeof(int));
  for (int i = 0; i < thread_arg->n_items_to_consume; ++i) {
    received_items[i] = channel_recv(thread_arg->channel, thread_arg->id);
    // sleep to pretend some work is being done to process the item
    sleep(rand() % 4);
  }

  printf("Consumer #%d finished. Items recieved: ", thread_arg->id);
  for (int i = 0; i < thread_arg->n_items_to_consume - 1; ++i)
    printf("%d, ", received_items[i]);
  printf("%d\n", received_items[thread_arg->n_items_to_consume - 1]);

  return NULL;
}

int main(int argc, char *argv[]) {
  assert_value(argc == 4,
               "Please pass number of producers, number of consumers, and the "
               "buffer_size. E.g.: ./main 3 2 10");

  srand(time(NULL)); // seed for randomness

  int n_producers = atoi(argv[1]);
  int n_consumers = atoi(argv[2]);
  int buffer_size = atoi(argv[3]);

  printf("\nUsing:\n"
         "%d producers\n"
         "%d consumers\n"
         "%d buffer slots\n"
         "-----------------\n",
         n_producers, n_consumers, buffer_size);

  Channel channel;
  channel_init(&channel, buffer_size);

  // create two thread pools, one for producers and the other for consumers
  pthread_t *producers = (pthread_t *)malloc(n_producers * sizeof(pthread_t));
  pthread_t *consumers = (pthread_t *)malloc(n_consumers * sizeof(pthread_t));

  // create the arguments that will be provided to the thread functions
  ThreadArg *producers_args =
      (ThreadArg *)malloc(n_producers * sizeof(ThreadArg));
  ThreadArg *consumers_args =
      (ThreadArg *)malloc(n_consumers * sizeof(ThreadArg));

  int error_code;

  // spawn producers threads
  for (int i = 0; i < n_producers; ++i) {
    producers_args[i].channel = &channel;
    producers_args[i].id = i;
    error_code = pthread_create(&producers[i], NULL, producer_routine,
                                &producers_args[i]);
    assert_value(error_code == 0, "Failed to create producer thread");
  }

  // spawn consumers threads
  for (int i = 0; i < n_consumers; ++i) {
    consumers_args[i].channel = &channel;
    consumers_args[i].id = i;
    consumers_args[i].n_items_to_consume =
        n_producers * NUM_OF_ITEMS_TO_PRODUCE_PER_THREAD / n_consumers;

    error_code = pthread_create(&consumers[i], NULL, consumer_routine,
                                &consumers_args[i]);
    assert_value(error_code == 0, "Failed to create consumer thread");
  }

  // join on all producers threads
  for (int i = 0; i < n_producers; ++i) {
    error_code = pthread_join(producers[i], NULL);
    assert_value(error_code == 0, "Couldn't join producer thread");
  }

  // join on all consumers threads
  for (int i = 0; i < n_consumers; ++i) {
    error_code = pthread_join(consumers[i], NULL);
    assert_value(error_code == 0, "Couldn't join consumer thread");
  }

  channel_destroy(&channel);

  // deallocates the heap allocations
  free(producers);
  free(consumers);
  free(producers_args);
  free(consumers_args);

  return 0;
}
