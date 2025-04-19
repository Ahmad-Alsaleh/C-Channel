#include <stdio.h>

#include "channel.h"
#include "deque.h"
#include "utils.h"

void test_send_once_works() {
  Channel channel;
  channel_init(&channel, 3);

  channel_send(&channel, 0, false);
  assert_value(channel.buffer.len == 1, "channel.len != 1");
  assert_value(channel.buffer.buffer[0] == 0, "buffer[0] != 0");

  channel_send(&channel, 1, false);
  assert_value(channel.buffer.len == 2, "channel.len != 2");
  assert_value(channel.buffer.buffer[1] == 1, "buffer[1] != 1");

  channel_destroy(&channel);
}

void test_send_blocks() {
  printf("[WARNING] this function will block your program infinitely\n");

  Channel channel;
  channel_init(&channel, 1);

  channel_send(&channel, 0, false);
  channel_send(&channel, 1, false);

  channel_destroy(&channel);
}

void test_recv_once_works() {
  Channel channel;
  channel_init(&channel, 2);

  channel_send(&channel, 0, false);
  channel_send(&channel, 1, false);

  int item;
  item = channel_recv(&channel, false);
  assert_value(item == 0, "item != 1");

  item = channel_recv(&channel, false);
  assert_value(item == 1, "item != 0");

  channel_destroy(&channel);
}

void test_recv_blocks() {
  printf("[WARNING] this function will block your program infinitely\n");
  Channel channel;
  channel_init(&channel, 2);

  channel_send(&channel, 0, false);

  int item;
  item = channel_recv(&channel, false);
  assert_value(item == 0, "item != 1");

  item = channel_recv(&channel, false);

  channel_destroy(&channel);
}

void test_deque_push_works() {
  Deque deque;
  deque_init(&deque, 3);

  for (int i = 0; i < 3; ++i)
    deque_push(&deque, i);

  assert_value(deque.len == 3, "deque.len != 3");

  for (int i = 0; i < 3; ++i)
    assert_value(deque.buffer[i] == i, "deque.buffer[i] != i");
}

void test_deque_pop_works() {
  Deque deque;
  deque_init(&deque, 3);

  for (int i = 0; i < 3; ++i)
    deque_push(&deque, i);

  for (int i = 0; i < 3; ++i)
    assert_value(deque_pop(&deque) == i, "deque_pop(&deque) != i");

  assert_value(deque.len == 0, "deque.len != 0");
}

void test_deque_circular_push_pop() {
  Deque deque;
  deque_init(&deque, 3);

  deque_push(&deque, 'a');
  deque_push(&deque, 'b');
  deque_push(&deque, 'c');

  assert_value(deque_pop(&deque) == 'a', "deque_pop(&deque) != 'a'");

  deque_push(&deque, 'd');

  assert_value(deque_pop(&deque) == 'b', "deque_pop(&deque) != 'b'");
  assert_value(deque_pop(&deque) == 'c', "deque_pop(&deque) != 'c'");
  assert_value(deque_pop(&deque) == 'd', "deque_pop(&deque) != 'd'");
}

int main(int argc, char *argv[]) {
  printf("Starting tests...\n");

  test_deque_push_works();
  test_deque_pop_works();
  test_deque_circular_push_pop();
  test_send_once_works();
  test_recv_once_works();
  // test_send_blocks();
  // test_recv_blocks();

  printf("Passed all tests!\n");
  return 0;
}
