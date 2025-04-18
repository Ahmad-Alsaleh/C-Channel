#include <stdio.h>
#include <stdlib.h>

#include "channel.h"

int main(int argc, char *argv[]) {
  printf("Hi...\n");

  Channel channel;
  channel_init(&channel, 3);
  channel_destroy(&channel);

  return 0;
}
