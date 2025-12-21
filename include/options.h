#ifndef LG_OPTIONS_H
#define LG_OPTIONS_H

#include "filters.h"

typedef struct {
	time_filter *tfil;
	mssg_filter *mfil;
	bool has_version;
	bool has_help;
	bool has_tfil;
	bool has_mfil;
	bool has_colour;
} options_t;

options_t *get_options(int argc, char *argv[]);
void free_options(options_t *options);

#endif
