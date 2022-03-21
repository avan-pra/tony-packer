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
	int fd;
	unsigned char *mptr;
	t_args *args;

	if (argc < 2)
		return help();

	if ((args = getargs(argc, argv)) == NULL)
		return 1; // error will already have been printed

	if ((fd = open(args->exe_name, O_RDONLY)) == -1) {
		print_error(args->exe_name);
		free(args);
		return 1;
	}

	// get length of exe
	struct stat statbuf;
	syscall(SYS_fstat, fd, &statbuf);

	if ((mptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		print_error("mmap");
		free(args);
		return 1;
	}

	close(fd);

	// printf("%s\n", mptr);

	if (munmap(mptr, statbuf.st_size) != 0) {
		print_error(NULL);
		free(args);
		return 1;
	}

	free(args);
	return 0;
}
