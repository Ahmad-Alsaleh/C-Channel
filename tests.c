#include <stdio.h>

#include "channel.h"
#include "deque.h"
#include "utils.h"
#include "priorityQueue.h"

void test_send_once_works() {
  Channel channel;
  channel_init(&channel, 3);

  channel_send(&channel, 0, -1);
  assert_value(channel.buffer.len == 1, "channel.len != 1");
  assert_value(channel.buffer.buffer[0] == 0, "buffer[0] != 0");

  channel_send(&channel, 1, -1);
  assert_value(channel.buffer.len == 2, "channel.len != 2");
  assert_value(channel.buffer.buffer[1] == 1, "buffer[1] != 1");

  channel_destroy(&channel);
}

void test_send_blocks() {
  printf("[WARNING] this function will block your program infinitely\n");

  Channel channel;
  channel_init(&channel, 1);

  channel_send(&channel, 0, -1);
  channel_send(&channel, 1, -1);

  channel_destroy(&channel);
}

void test_recv_once_works() {
  Channel channel;
  channel_init(&channel, 2);

  channel_send(&channel, 0, -1);
  channel_send(&channel, 1, -1);

  int item;
  item = channel_recv(&channel, -1);
  assert_value(item == 0, "item != 1");

  item = channel_recv(&channel, -1);
  assert_value(item == 1, "item != 0");

  channel_destroy(&channel);
}

void test_recv_blocks() {
  printf("[WARNING] this function will block your program infinitely\n");
  Channel channel;
  channel_init(&channel, 2);

  channel_send(&channel, 0, -1);

  int item;
  item = channel_recv(&channel, -1);
  assert_value(item == 0, "item != 1");

  item = channel_recv(&channel, -1);

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

//..........................
// Test insert and extract_max functionality
void test_insert_and_extract_max() {
  PriorityQueue pq;
  create_pq(&pq, 10);

  int val1 = 100, val2 = 200, val3 = 300;

  insert(&pq, &val1, 3); // Insert with priority 3
  insert(&pq, &val2, 1); // Insert with priority 1
  insert(&pq, &val3, 2); // Insert with priority 2

  // Extract max should return the item with highest priority (val1)
  int *item = extract_max(&pq);
  assert_value(*item == 100 , "Expected 100, but got a different value.");

  // Extract max again should return val3 (priority 2)
  item = extract_max(&pq);
  assert_value(*item == 300 , "Expected 300, but got a different value.");

  // Finally, val2 (priority 1) should be returned
  item = extract_max(&pq);
  assert_value(*item == 200 , "Expected 200, but got a different value.");

  // At this point, the priority queue should be empty
  assert_value(pq.size == 0 , "Priority queue should be empty.");

  destroy(&pq);
}

void test_empty_queue() {
  PriorityQueue pq;
  create_pq(&pq, 10);
  // Try extracting from an empty priority queue (should assert)
  int *item = extract_max(&pq); // This should trigger an assertion failure
  // If it doesn't, add an assert to check this case
  
  destroy(&pq);
}

// Test inserting into a full priority queue (asserting failure)
void test_full_queue() {
  PriorityQueue pq;
  create_pq(&pq, 2);  // Queue of size 2

  int val1 = 100, val2 = 200;

  insert(&pq, &val1, 3);
  insert(&pq, &val2, 1);
  
  // Try inserting into a full queue (this should fail)
  int val3 = 300;
  int result = insert(&pq, &val3, 2);
  assert_value(result == -1 , "Insert should fail when queue is full.");

  destroy(&pq);
}
//.........................

int main() {
  printf("Starting tests...\n");

  test_deque_push_works();
  test_deque_pop_works();
  test_deque_circular_push_pop();
  test_send_once_works();
  test_recv_once_works();
  // test_send_blocks();
  // test_recv_blocks();


  test_insert_and_extract_max();  // Test normal insert and extract functionality
  test_empty_queue();             // Test extracting from empty queue
  test_full_queue();              // Test inserting into a full queue



  printf("Passed all tests!\n");
  return 0;
}
