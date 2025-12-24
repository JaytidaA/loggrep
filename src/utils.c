#include "config.h"
#include "utils.h"

struct arg_info {
	char *progname;
	FILE *input;
};

static struct arg_info g_info;

void print_help(void)
{
	puts(LOGGREP_HELPSTR);
}

void print_onexit(int status, void *arg)
{
	struct arg_info *info = arg;
	if (info->input && info->input != stdin)
		fclose(info->input);
	if (status == ERR_MALFORMED_INPUT ||
		status == ERR_OUT_OF_RANGE_INPUT ||
		status == ERR_UNKNOWN_OPTION)
		fprintf(stderr, "%s: run -h for help.\n", info->progname);
}

void initialise(char *progname, FILE *input)
{
	g_info = (struct arg_info) { progname, input };
	int s = on_exit(print_onexit, (void *) &g_info);
	if (s)
		perror("initialise: on_exit");
}
