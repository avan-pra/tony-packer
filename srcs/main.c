#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "args.h"

int help()
{
	printf("Usage: `tony-packer [options] <binary>` (-h for help)\n");
	return 1;
}

int full_help()
{
	printf("\n-h / --help				| show help\n\
-k <key> / --key <key>			| use custom key \n");
	return 1;
}

int print_error(char *str)
{
	if (str == NULL)
		printf("%s%s\n", ERRORTEMPLATE, strerror(errno));
	else
		printf("%s%s\n", ERRORTEMPLATE, str);
	return 1;
}

enum argID
{
	eEXE_NAME,
	eKEY,
	eHELP,
	eUNKNOWN
};

enum argID switch_str_arg(char *str)
{
	if (strcmp(str, "-k") == 0)	return eKEY;
	if (strcmp(str, "-h") == 0 || strcmp(str, "--help") == 0) return eHELP;

	return eUNKNOWN;
}

t_args *getargs(int argc, char **argv)
{
	t_args *args;

	if (!(args = malloc(sizeof(t_args)))) {
		print_error(NULL);
		return NULL;
	}
	memset(args, 0, sizeof(t_args));
	
	for (int i = 1; i < argc ; ++i) {
		switch (switch_str_arg(argv[i])) {
			/* -k <key> */
			case eKEY: { if (i + 1 == argc) { free(args); print_error("-k: expected key as next argument"); return NULL; } else { args->key = argv[i + 1]; ++i; } break; }
			/* -h --help */
			case eHELP: { help(); full_help(); free(args); return NULL; break; }
			/* binary or unknown argument */
			default: { if (i == argc - 1) { args->exe_name = argv[argc - 1]; } else { free(args); printf("%s%s: unknown argument\n", ERRORTEMPLATE, argv[i]); return NULL; } break; }
		}
	}

	if (args->exe_name == NULL) {
		print_error("binary not supplied");
		free(args);
		return NULL;
	}

	return args;
}

int main(int argc, char **argv)
{
	int fd;
	t_args *args;

	if (argc < 2)
		return help();

	if ((args = getargs(argc, argv)) == NULL)
		return 1; // error will already have been printed

	if ((fd = open(args->exe_name, 0)) == -1)
		return print_error(NULL);
	//mmap
	close(fd);

	return 0;
}
