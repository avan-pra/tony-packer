#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/random.h>
#include "args.h"
#include "utils.h"

enum argID
{
	eEXE_NAME,
	eKEY,
	eHELP,
	eVERBOSE,
	eOUTPUT,
	eUNKNOWN
};

enum argID switch_str_arg(char *str)
{
	if (strcmp(str, "-k") == 0 || strcmp(str, "--key") == 0)			return eKEY;
	else if (strcmp(str, "-h") == 0 || strcmp(str, "--help") == 0)		return eHELP;
	else if (strcmp(str, "-v") == 0 || strcmp(str, "--verbose") == 0)	return eVERBOSE;
	else if (strcmp(str, "-o") == 0 || strcmp(str, "--output") == 0)	return eOUTPUT;
	return eUNKNOWN;
}

/* use getopt instead of a shitty handmade argument parser*/
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
			/* -v --verbose */
			case eVERBOSE: { args->verbose = 1; break; }
			/* -o <key> */
			case eOUTPUT: { if (i + 1 == argc) { free(args); print_error("-o: expected output file as next argument"); return NULL; } else { args->out_exe = argv[i + 1]; ++i; } break; }
			/* binary or unknown argument */
			default: { if (i == argc - 1) { args->exe_name = argv[argc - 1]; } else { free(args); printf("%s%s: unknown argument\n", ERRORTEMPLATE, argv[i]); return NULL; } break; }
		}
	}

	if (args->verbose) { printf("Using verbose\n"); }

	if (args->exe_name == NULL) {
		print_error("binary not supplied");
		free(args);
		return NULL;
	}
	if (args->verbose) { printf("Using input file `%s`\n", args->exe_name); }

	if (args->out_exe == NULL)
		args->out_exe = DEFAULTOFNAME;
	if (args->verbose) { printf("Using output file `%s`\n", args->out_exe); }

	/* error if input == output */
	if (strcmp(args->exe_name, args->out_exe) == 0) {
		printf("%s%s`%s`%s\n", ERRORTEMPLATE, "input file must be different from output file (using ", DEFAULTOFNAME, " as output file by default)");
		free(args);
		return NULL;
	}

	if (args->key == NULL) {
		char random_buffer[32];
		if (getrandom(random_buffer, 32, GRND_NONBLOCK) == -1) {
			print_error(NULL);
			free(args);
			return NULL;
		}
		for (int i = 0; i < 32; i++) // convert urand0m 32 char to a 64 hex (can probably be done without a for but idk)
			sprintf(&args->random_key[i * 2], "%02x", (unsigned char)random_buffer[i]);
		args->key = args->random_key;
		if (args->verbose) { printf("No key supplied, generating...\n"); }
	}
	printf("key_value: %s\n", args->key);

	return args;
}