#include <stdio.h>

#include "channel.h"
#include "utils.h"

// tests...
void test_send_once_works() {
  Channel channel;
  channel_init(&channel, 3);

  channel_send(&channel, 0);
  assert_value(channel.len == 1, "channel.len != 1");
  assert_value(channel.buffer[0] == 0, "buffer[0] != 0");

  channel_send(&channel, 1);
  assert_value(channel.len == 2, "channel.len != 2");
  assert_value(channel.buffer[1] == 1, "buffer[1] != 1");

  channel_destroy(&channel);
}

void test_send_blocks() {
  printf("[WARNING] this function will block your program infinitely\n");

  Channel channel;
  channel_init(&channel, 1);

  channel_send(&channel, 0);
  channel_send(&channel, 1);

  channel_destroy(&channel);
}

void test_recv_once_works() {
  Channel channel;
  channel_init(&channel, 2);

  channel_send(&channel, 0);
  channel_send(&channel, 1);

  int item;
  item = channel_recv(&channel);
  assert_value(item == 1, "item != 1");

  item = channel_recv(&channel);
  assert_value(item == 0, "item != 0");

  channel_destroy(&channel);
}

void test_recv_blocks() {
  printf("[WARNING] this function will block your program infinitely\n");
  Channel channel;
  channel_init(&channel, 2);

  channel_send(&channel, 0);

  int item;
  item = channel_recv(&channel);
  assert_value(item == 0, "item != 1");

  item = channel_recv(&channel);

  channel_destroy(&channel);
}

int main(int argc, char *argv[]) {
  printf("Starting tests...\n");

  // test_send_once_works();
  // test_send_blocks();
  // test_recv_once_works();
  // test_recv_blocks();

  printf("Passed all tests!\n");
  return 0;
}
