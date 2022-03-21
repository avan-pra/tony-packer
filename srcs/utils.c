#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "args.h"

int print_error(char *str)
{
	if (str == NULL)
		printf("%s%s\n", ERRORTEMPLATE, strerror(errno));
	else if (errno != 0)
        printf("%s%s %s\n", ERRORTEMPLATE, str, strerror(errno));
    else
		printf("%s%s\n", ERRORTEMPLATE, str);
	return 1;
}