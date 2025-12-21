#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int main(void)
{
	char *lineptr = NULL;
	size_t length = 0;
	ssize_t ret;

	while ((ret = getline(&lineptr, &length, stdin)) != -1) {
		if (lineptr) {
			lineptr[ret - 1] = '\0';
			printf("Got input: %s\n", lineptr);
		}
	}
	puts("File ended");
	return 0;
}
