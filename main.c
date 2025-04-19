#include <pthread.h>
#include <stdio.h>

#include "channel.h"

void *producer_routine(void *channel) {
  for (int i = 0; i < 20; ++i) {
    channel_send((Channel *)channel, i, true);
  }

  return NULL;
}

void *consumer_routine(void *channel) {
  while (true) {
    int item = channel_recv((Channel *)channel, true);
  }
}

int main(int argc, char *argv[]) {
  Channel channel;
  channel_init(&channel, 3);

  pthread_t producer_thread;
  pthread_create(&producer_thread, NULL, producer_routine, (void *)&channel);

  pthread_t consumer_thread;
  pthread_create(&consumer_thread, NULL, consumer_routine, (void *)&channel);

  pthread_join(producer_thread, NULL);
  pthread_join(consumer_thread, NULL);

  channel_destroy(&channel);

  return 0;
}
