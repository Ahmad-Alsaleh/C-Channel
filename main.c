#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "channel.h"

void *producer_1_routine(void *channel) {
  for (int i = 0; i < 5; ++i) {
    channel_send((Channel *)channel, i, "producer 1");
  }

  return NULL;
}

void *producer_2_routine(void *channel) {
  for (int i = 5; i < 10; ++i) {
    sleep(1); // pretend some work is being done
    channel_send((Channel *)channel, i, "producer 2");
  }

  return NULL;
}

void *consumer_routine(void *channel) {
  while (true) {
    int item = channel_recv((Channel *)channel, "consumer");
    sleep(2); // pretend some work is being done
  }
}

int main(int argc, char *argv[]) {
  Channel channel;
  channel_init(&channel, 3);

  pthread_t producer_1_thrd;
  pthread_create(&producer_1_thrd, NULL, producer_1_routine, (void *)&channel);

  pthread_t producer_2_thrd;
  pthread_create(&producer_2_thrd, NULL, producer_2_routine, (void *)&channel);

  pthread_t consumer_thrd;
  pthread_create(&consumer_thrd, NULL, consumer_routine, (void *)&channel);

  pthread_join(producer_1_thrd, NULL);
  pthread_join(producer_2_thrd, NULL);
  pthread_join(consumer_thrd, NULL);

  channel_destroy(&channel);

  return 0;
}
