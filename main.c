#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

#include "config.h"
#include "utils.h"
#include "filters.h"
#include "options.h"
#include "colours.h"

int main(int argc, char *argv[])
{
	char *lineptr = NULL;
	size_t length = 0;
	ssize_t ret;

	options_t *opts = get_options(argc, argv);
	time_filter *tfil = NULL;
	mssg_filter *mfil = NULL;
	FILE *input = NULL;

	if (opts->has_version) {
		printf("%s %s\n", argv[0], LOGGREP_VERSION);
		free_options(opts);
		return EXIT_SUCCESS;
	}
	if (opts->has_help) {
		print_help();
		free_options(opts);
		return EXIT_SUCCESS;
	}

	if (opts->has_tfil)
		tfil = opts->tfil;
	if (opts->has_mfil)
		mfil = opts->mfil;
	if (!tfil && !mfil) {
		free_options(opts);
		fprintf(stderr, "%s: no filters specified, exiting early.\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (opts->has_file)
		input = fopen(opts->filename, "r");
	else
		input = stdin;
	if (!input) {
		free_options(opts);
		perror("fopen");
		return EXIT_FAILURE;
	}

	initialise(argv[0], input);
	struct tm tm = {0};
	char *next = NULL;

	while ((ret = getline(&lineptr, &length, input)) != -1) {
		// If no allocation
		if (!lineptr)
			continue;

		lineptr[ret - 1] = '\0';

		next = strptime(lineptr, DATETIME_STR, &tm);
		if (!next) {
			fprintf(stderr, "%s: unexpected date time string format!\n", argv[0]);
			free_options(opts);
			exit(ERR_MALFORMED_INPUT);
		}

		// Timing checks
		bool pass_tfil;
		if (tfil) {
			pass_tfil = pass_time_filter(tfil, &tm);
			tm = (struct tm) {0};
			if (!pass_tfil)
				continue;
		}
		
		// Message checks
		pmft pass_mfil = {false, 0, NULL};
		if (mfil) {
			pass_mfil = pass_mssg_filter(mfil, next);
			if (!pass_mfil.pass) {
				// No need to free now as no allocations were made
				continue;
			}
		}

		if (opts->has_colour)
			print_colour(
				lineptr, ret - 1, pass_mfil.matches,
				pass_mfil.matchlen, opts->has_tfil
			);
		else
			puts(lineptr);
	}

	// Handle getline error
	if (errno) {
		perror("getline");
		free_options(opts);
		return EXIT_FAILURE;
	}

	free(lineptr);
	free_options(opts);
	return 0;
}
