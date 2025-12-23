#ifndef LG_COLOURS_H
#define LG_COLOURS_H

#include "filters.h"

#include <stdbool.h>
#include <sys/types.h>

// ansi-escape red-bold colour
#define COLOUR_START "\x1b[91m\x1b[1m"
// ansi-escape reset
#define COLOUR_END   "\x1b[0m"

// Print the logline with appropriate parts highlighted as a colour.
// NOTE: `n` is an int because the getline function expects an int *n argument.
void print_colour(const char *logline, int n, strmatch_t *matches, size_t matchlen, bool has_tfil);

#endif 
