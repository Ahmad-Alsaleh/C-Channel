#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "channel.h"
#include "utils.h"

typedef struct {
  Channel *channel;
  int id;
} Foo;

void *fast_producer_routine(void *channel) {
  for (int i = 0; i < 5; ++i) {
    channel_send((Channel *)channel, i, "fast producer");
  }
  return NULL;
}

void *slow_producer_routine(void *channel) {
  for (int i = 5; i < 100; ++i) {
    sleep(1); // pretend some work is being done to generate the item
    channel_send((Channel *)channel, i, "slow producer");
  }
  return NULL;
}

void *fast_consumer_routine(void *channel) {
  while (true) {
    channel_recv((Channel *)channel, "fast consumer");
  }
}

void *slow_consumer_routine(void *channel) {
  while (true) {
    channel_recv((Channel *)channel, "slow consumer");
    sleep(2); // pretend some work is being done to process the item
  }
}

int main(int argc, char *argv[]) {
  assert_value(argc == 4,
               "Please pass number of producers, number of consumers, and the "
               "buffer_size. E.g.: ./main 3 2 10");

  int n_producers = atoi(argv[1]);
  int n_consumers = atoi(argv[2]);
  int buffer_size = atoi(argv[3]);

  int n_fast_producers = n_producers / 2;
  int n_slow_producers = n_producers - n_fast_producers;

  int n_fast_consumers = n_consumers / 2;
  int n_slow_consumers = n_consumers - n_fast_consumers;

  printf("\nUsing:\n"
         "%d producers (%d fast and %d slow)\n"
         "%d consumers (%d fast and %d slow)\n"
         "%d buffer slots\n"
         "------------------------------------\n",
         n_producers, n_fast_producers, n_slow_producers, n_consumers,
         n_fast_consumers, n_slow_consumers, buffer_size);

  Channel channel;
  channel_init(&channel, buffer_size);

  pthread_t *thread_pool =
      (pthread_t *)malloc((n_producers + n_consumers) * sizeof(pthread_t));

  int i = 0;
  for (int j = 0; j < n_fast_producers; ++j)
    pthread_create(&thread_pool[i++], NULL, fast_producer_routine, &channel);

  for (int j = 0; j < n_slow_producers; ++j)
    pthread_create(&thread_pool[i++], NULL, slow_producer_routine, &channel);

  for (int j = 0; j < n_fast_consumers; ++j)
    pthread_create(&thread_pool[i++], NULL, fast_consumer_routine, &channel);

  for (int j = 0; j < n_slow_consumers; ++j)
    pthread_create(&thread_pool[i++], NULL, slow_consumer_routine, &channel);

  for (int i = 0; i < n_producers + n_consumers; ++i)
    pthread_join(thread_pool[i], NULL);

  channel_destroy(&channel);

  return 0;
}
