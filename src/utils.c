#include "config.h"
#include "utils.h"

#include <stdio.h>

void print_help(void)
{
	const char *helpstr = LOGGREP_HELPSTR;
	puts(helpstr);
}

void print_onexit(int status, void *arg)
{
	char *pn = (char *) arg;
	if (status == ERR_MALFORMED_INPUT ||
		status == ERR_OUT_OF_RANGE_INPUT ||
		status == ERR_UNKNOWN_OPTION)
		fprintf(stderr, "%s: run -h for help.\n", pn);
}

void initialise(char *progname)
{
	int s = on_exit(print_onexit, (void *) progname);
	if (s) {
		perror("initialise: on_exit");
	}
}
