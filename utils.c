#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void assert_value(bool expression, const char *error_msg) {
  if (!expression) {
    fprintf(stderr, "[ERROR] %s\n", error_msg);
    exit(1);
  }
}
