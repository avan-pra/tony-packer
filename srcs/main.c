#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "args.h"
#include "utils.h"

int main(int argc, char **argv)
{
	t_args *args;

	if (argc < 2)
		return help();

	if ((args = getargs(argc, argv)) == NULL)
		return 1; // error will already have been printed

	/* pack() will take care of freeing args */
	return pack(args);
}
