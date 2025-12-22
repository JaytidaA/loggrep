#ifndef LG_FILTERS_H
#define LG_FILTERS_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	bool has_before; // Before or on a specific timestamp
	bool has_after;  // After  or on a specific timestamp
	time_t before;
	time_t after;

	bool has_before_time; // Before or on a specific second in the day
	bool has_after_time;  // After  or on a specific second in the day
	int before_time, after_time;

	bool has_hour;
	int hour;

	bool has_minute;
	int minute;
} time_filter;

typedef struct {
	size_t length;
	char *message;
} mssg_filter;

/* Taking inspiration from regmatch_t.
   See <regex.h> */
typedef struct {
	size_t so_sm;
	size_t eo_sm;
} strmatch_t;

#ifdef DEBUG
void print_time_filter(time_filter *tfil);
void print_mssg_filter(mssg_filter *mfil);
#endif

bool pass_time_filter(time_filter *tfil, struct tm *tm);

/* Container to store the result of pass_mssg_filter */
typedef struct {
	bool pass;
	size_t matchlen;
	strmatch_t *matches;
} pass_mssg_filter_t;

/* Helper type because I am too lazy to write out the entire
   name of the type. This type is always returned by value. */
typedef pass_mssg_filter_t pmft;

pass_mssg_filter_t pass_mssg_filter(mssg_filter *mfil, char *log);
void free_pmft(pass_mssg_filter_t pmft);

#endif
