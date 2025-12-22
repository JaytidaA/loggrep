#define _XOPEN_SOURCE
#include "filters.h"
#include "config.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG

void print_time_filter(time_filter *tfil)
{
	if (tfil->has_after)
		printf("after : %ld\n", tfil->after);
	if (tfil->has_before)
		printf("before: %ld\n", tfil->before);
	if (tfil->has_after_time)
		printf("after time : %ds\n", tfil->after_time);
	if (tfil->has_before_time)
		printf("before time: %ds\n", tfil->before_time);
	if (tfil->has_hour)
		printf("hour  : %d\n", tfil->hour);
	if (tfil->has_minute)
		printf("minute: %d\n", tfil->minute);
}

void print_mssg_filter(mssg_filter *mfil)
{
	printf("search for: ");
	puts(mfil->message);
}

#endif

bool pass_time_filter(time_filter *tfil, struct tm *tm)
{
	struct tm temp = *tm;
	time_t unified = mktime(&temp);
	int seconds = tm->tm_sec + 60 * (tm->tm_min + 60 * (tm->tm_hour));

	if (tfil->has_after && !(unified >= tfil->after))
		return false;
	if (tfil->has_before && !(unified <= tfil->before))
		return false;
	if (tfil->has_after_time && !(seconds >= tfil->after_time))
		return false;
	if (tfil->has_before_time && !(seconds <= tfil->before_time))
		return false;
	if (tfil->has_hour && (tm->tm_hour != tfil->hour))
		return false;
	if (tfil->has_minute && (tm->tm_min != tfil->minute))
		return false;

	return true;
}

pass_mssg_filter_t pass_mssg_filter(mssg_filter *mfil, char *log)
{
	size_t len = 0;
	strmatch_t *matches = NULL;
	char *substart;
	while ((substart = strstr(log, mfil->message))) {
		len++;
		matches = (strmatch_t *) realloc(matches, len * sizeof(strmatch_t));
		if (!matches) {
			perror("pass_mssg_filter: realloc");
			// handle conservatively
			return (pass_mssg_filter_t) {false, 0, NULL}; 
		}

		matches[len - 1].so_sm = (size_t) (substart - log);
		matches[len - 1].eo_sm = (size_t) (substart - log + mfil->length);

		log = &log[matches[len - 1].eo_sm];
	}

	if (len)
		 return (pass_mssg_filter_t) {true, len, matches};
	else
		return (pass_mssg_filter_t) {false, 0, NULL};
}

void free_pmft(pass_mssg_filter_t pmft)
{
	// pmft only contains the array to be deallocated
	// which is a pointer that is copied.
	free(pmft.matches);
}
