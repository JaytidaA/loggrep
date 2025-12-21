#define _XOPEN_SOURCE
#include "filters.h"
#include "config.h"

#include <time.h>

#ifdef DEBUG
#include <stdio.h>

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
	return true;
}
