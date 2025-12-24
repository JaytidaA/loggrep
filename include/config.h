#ifndef LG_CONFIG_H
#define LG_CONFIG_H

#define DATETIME_STR "%Y-%m-%d %T"
#define DATE_STR "%Y-%m-%d"
#define TIME_STR "%T"
#define HOUR_STR "%H"
#define MINT_STR "%M"

#define PRINT_STR_SIZE 79
#define LOG_STR_SIZE INT32_MAX - 1

// Error codes (EXIT_FAILURE = 1)
#define ERR_NULL_MALLOC        2
#define ERR_MALFORMED_INPUT    3
#define ERR_OUT_OF_RANGE_INPUT 4
#define ERR_UNKNOWN_OPTION     5
#define ERR_EMPTY_SET          6

#define LOGGREP_HELPSTR \
	("Usage: loggrep [options] [-m message] [-f file]\n"\
	"\n"\
	"A singular line in the logfile is expected to have the format:\n"\
	"%Y-%m-%d %H:%M:%S (e.g. 1970-01-01 00:00:00)\n"\
	"\n"\
	"Options:\n"\
	"-m: The message to filter logs by\n"\
	"-f: The logfile to search for (default stdin)\n"\
	"-c: Colorise output (default off)\n"\
	"-a: After DateTime (format %Y-%m-%d %H:%M:%S)\n"\
	"-b: Before DateTime (format %Y-%m-%d %H:%M:%S)\n"\
	"-d: After date (format %Y-%m-%d)\n"\
	"-D: Before date (format %Y-%m-%d)\n"\
	"-t: After time (format %H:%M:%S)\n"\
	"-T: Before (format %H:%M:%S)\n"\
	"-H: Hour of day to filter by (range 0-23)\n"\
	"-M: Minute of hour to filter by (range 0-59)\n"\
	"-h: Show this help\n"\
	"-v: Show the version number\n")

// use semantic versioning
#define LOGGREP_VERSION \
	"version: 0.1.0"

#endif 
