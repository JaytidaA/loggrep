#define _XOPEN_SOURCE // Required for strptime
#include "options.h"
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void handle_after(char *print, char *fmt, char *optarg, bool *has_after, time_t *after, char *progname);
void handle_before(char *print, char *fmt, char *optarg, bool *has_before, time_t *before, char *progname);
void handle_after_time(char *print, char *fmt, char *optarg, bool *has_after, int *after, char *progname);
void handle_before_time(char *print, char *fmt, char *optarg, bool *has_before, int *before, char *progname);

options_t *get_options(int argc, char *argv[])
{
	int opt;

	options_t *ret = (options_t *) calloc(1, sizeof(options_t));
	if (!ret) {
		perror("get_options: calloc");
		return NULL;
	}

	ret->tfil = (time_filter *) calloc(1, sizeof(time_filter));
	if (!ret->tfil) {
		perror("get_options: calloc");
		free(ret);
		return NULL;
	}

	ret->mfil = (mssg_filter *) calloc(1, sizeof(mssg_filter));
	if (!ret->mfil) {
		perror("get_options: calloc");
		free(ret->tfil);
		free(ret);
		return NULL;
	}

	time_filter *tfil = ret->tfil;
	mssg_filter *mfil = ret->mfil;

	char *endptr;

	while ((opt = getopt(argc, argv, "a:b:d:D:t:T:H:M:m:chvf:")) != -1) {
		switch (opt) {
			// Time based filtering
			case 'a':
				ret->has_tfil = true;
				handle_after("datetime", DATETIME_STR, optarg, &tfil->has_after, &tfil->after, argv[0]);
				break;
			case 'b':
				ret->has_tfil = true;
				handle_before("datetime", DATETIME_STR, optarg, &tfil->has_before, &tfil->before, argv[0]);
				break;
			case 'd':
				ret->has_tfil = true;
				handle_after("date", DATE_STR, optarg, &tfil->has_after, &tfil->after, argv[0]);
				break;
			case 'D':
				ret->has_tfil = true;
				handle_before("date", DATE_STR, optarg, &tfil->has_before, &tfil->before, argv[0]);
				break;
			case 't':
				ret->has_tfil = true;
				handle_after_time("time", TIME_STR, optarg, &tfil->has_after_time, &tfil->after_time, argv[0]);
				break;
			case 'T':
				ret->has_tfil = true;
				handle_before_time("time", TIME_STR, optarg, &tfil->has_before_time, &tfil->before_time, argv[0]);
				break;
			case 'H':
				ret->has_tfil = true;
				tfil->has_hour = true;
				tfil->hour = (int) strtol(optarg, &endptr, 10);
				if (*endptr != '\0') {
					fprintf(stderr, "%s: malformed input for hour.\n", argv[0]);
					exit(ERR_MALFORMED_INPUT);
				}

				if (tfil->hour < 0 || tfil->hour > 23) {
					fprintf(stderr, "%s: out of range input for hour.\n", argv[0]);
					exit(ERR_OUT_OF_RANGE_INPUT);
				}

				break;
			case 'M':
				ret->has_tfil = true;
				tfil->has_minute = true;
				tfil->minute = (int) strtol(optarg, &endptr, 10);
				if (*endptr != '\0') {
					fprintf(stderr, "%s: malformed input for minute.\n", argv[0]);
					exit(ERR_MALFORMED_INPUT);
				}

				if (tfil->minute < 0 || tfil->minute > 59) {
					fprintf(stderr, "%s: out of range input for minute.\n", argv[0]);
					exit(ERR_OUT_OF_RANGE_INPUT);
				}

				break;

			// Message based filtering
			case 'm':
				ret->has_mfil = true;
				size_t arglen = strlen(optarg);
				mfil->length = ((arglen <= LOG_STR_SIZE) ? arglen : LOG_STR_SIZE);
				mfil->message = malloc(mfil->length + 1);
				if (!mfil->message) {
					perror("get_mssg_filter: malloc");
					free(ret->tfil);
					free(ret->mfil);
					free(ret);
					return NULL;
				}

				memcpy(mfil->message, optarg, mfil->length);
				mfil->message[mfil->length] = '\0';
				break;
			case 'c':
				ret->has_colour = true;
				break;
			case 'h':
				ret->has_help = true;
				break;
			case 'v':
				ret->has_version = true;
				break;
			case 'f':
				ret->has_file = true;
				ret->filename = optarg;
				break;
			default:
				exit(ERR_UNKNOWN_OPTION);
		}
	}

	if (tfil->after > tfil->before || tfil->after_time > tfil->before_time) {
		fprintf(stderr, "no logs considered valid\n");
		exit(ERR_EMPTY_SET);
	}

	return ret;
}

void handle_after(char *print, char *fmt, char *optarg, bool *has_after, time_t *after, char *progname)
{
	struct tm temp = {0};
	time_t temp_time;
	char *end;

	end = strptime(optarg, fmt, &temp);
	if (!end || *end != '\0') {
		fprintf(stderr, "%s: invalid or malformed input for %s.\n", progname, print);
		exit(ERR_MALFORMED_INPUT);
	}

	temp_time = mktime(&temp);
	if (!(*has_after)) {
		*has_after = true;
		*after = temp_time;
	} else if (*after < temp_time) {
		*after = temp_time;
	}
}

void handle_before(char *print, char *fmt, char *optarg, bool *has_before, time_t *before, char *progname)
{
	struct tm temp = {0};
	time_t temp_time;
	char *end;

	end = strptime(optarg, fmt, &temp);
	if (!end || *end != '\0') {
		fprintf(stderr, "%s: invalid or malformed input for %s.\n", progname, print);
		exit(ERR_MALFORMED_INPUT);
	}

	temp_time = mktime(&temp);
	if (!(*has_before)) {
		*has_before= true;
		*before = temp_time;
	} else if (*before > temp_time) {
		*before = temp_time;
	}
}

void handle_after_time(char *print, char *fmt, char *optarg, bool *has_after, int *after, char *progname)
{
	struct tm temp = {0};
	time_t temp_time;
	char *end;

	end = strptime(optarg, fmt, &temp);
	if (!end || *end != '\0') {
		fprintf(stderr, "%s: invalid or malformed input for %s.\n", progname, print);
		exit(ERR_MALFORMED_INPUT);
	}

	temp_time = temp.tm_sec + 60 * (temp.tm_min + 60 * temp.tm_hour);
	if (!(*has_after)) {
		*has_after = true;
		*after = temp_time;
	} else if (*after < temp_time) {
		*after = temp_time;
	}
}

void handle_before_time(char *print, char *fmt, char *optarg, bool *has_before, int *before, char *progname)
{
	struct tm temp = {0};
	time_t temp_time;
	char *end;

	end = strptime(optarg, fmt, &temp);
	if (!end || *end != '\0') {
		fprintf(stderr, "%s: invalid or malformed input for %s.\n", progname, print);
		exit(ERR_MALFORMED_INPUT);
	}

	temp_time = temp.tm_sec + 60 * (temp.tm_min + 60 * temp.tm_hour);
	if (!(*has_before)) {
		*has_before= true;
		*before = temp_time;
	} else if (*before < temp_time) {
		*before = temp_time;
	}
}

void free_options(options_t *options)
{
	free(options->mfil->message);
	free(options->tfil);
	free(options->mfil);
	free(options);
}
