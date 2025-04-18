#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// checks if `expression` is true; otherwise, prints an error message to
// `stderr` and exits with code 1
void assert_value(bool expression, const char *error_msg);

#endif
