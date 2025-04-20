#ifndef CONFIGS_H
#define CONFIGS_H

typedef enum {
  Fifo,
  Priority,
} Mode;

#define NUM_OF_ITEMS_TO_PRODUCE_PER_THREAD 10
#define MODE Priority // should be Priority or Fifo (case sensitive)

#endif
