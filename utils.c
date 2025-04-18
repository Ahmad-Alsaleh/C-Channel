#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// checks if `expression` is `true`, if not, an error message is printed to to
// `stderr` and the program exits with code 1
void assert_value(bool expression, const char *error_msg) {
  if (!expression) {
    fprintf(stderr, "[ERROR] %s\n", error_msg);
    exit(1);
  }
}
