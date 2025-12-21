#ifndef LG_CONFIG_H
#define LG_CONFIG_H

#define DATETIME_STR "%Y-%m-%d %T"
#define DATE_STR "%Y-%m-%d"
#define TIME_STR "%T"
#define HOUR_STR "%H"
#define MINT_STR "%M"

#define PRINT_STR_SIZE 79
#define LOG_STR_SIZE INT32_MAX - 1

// Error codes
#define ERR_NULL_MALLOC        1
#define ERR_MALFORMED_INPUT    2
#define ERR_OUT_OF_RANGE_INPUT 3
#define ERR_UNKNOWN_OPTION     4
#define ERR_EMPTY_SET          5

#endif 
