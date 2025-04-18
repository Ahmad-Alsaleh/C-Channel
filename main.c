#include <stdio.h>

#include "channel.h"

// TODO: use a deque instead of a vec for the buffer. make sure to replace
// `Channe.len` with two pointer and to change the implamentation of send and
// recv

// tests...
void test_send_once_works() {
  Channel channel;
  channel_init(&channel, 3);
  channel_send(&channel, 123);
  assert(channel.len == 1, "channel.len != 1");
  assert(channel.buffer[0] == 123, "buffer[0] != 123");
  channel_destroy(&channel);
}

// WARNING this  function will block your program infinitely
void test_send_blocks() {
  printf("[WARNING] this function will block your program infinitely");

  Channel channel;
  channel_init(&channel, 1);

  channel_send(&channel, 0);
  channel_send(&channel, 1);

  channel_destroy(&channel);
}

int main(int argc, char *argv[]) {
  printf("Starting...\n");

  test_send_once_works();
  // test_send_blocks();

  printf("Done...\n");
  return 0;
}

// tests...
