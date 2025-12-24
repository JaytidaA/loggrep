#define _XOPEN_SOURCE
#include "colours.h"
#include "config.h"

#include <time.h>
#include <stdio.h>

void print_colour(const char *log, int n, strmatch_t *match, size_t mlen, bool has_tfil)
{
	// this pointer will index over the entire log string
	size_t p = 0;
	struct tm ignore;

	char *next = strptime(log, DATETIME_STR, &ignore);

	size_t diff = (size_t) (next - log);

	if (has_tfil)
		printf(COLOUR_START);
	while (p < diff)
		putchar(log[p++]);
	if (has_tfil)
		printf(COLOUR_END);

	for (size_t i = 0; i < mlen; i++) {
		while ((p - diff) < match[i].so_sm)
			putchar(log[p++]);
		printf(COLOUR_START);
		while ((p - diff) < match[i].eo_sm)
			putchar(log[p++]);
		printf(COLOUR_END);
	}

	while (p < (size_t) n)
		putchar(log[p++]);
	putchar('\n');
}
