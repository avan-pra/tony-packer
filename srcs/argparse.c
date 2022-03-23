#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/random.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
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
	if (strncmp(str, "-k", 2) == 0)			return eKEY;
	else if (strncmp(str, "-h", 2) == 0)	return eHELP;
	else if (strncmp(str, "-v", 2) == 0)	return eVERBOSE;
	else if (strncmp(str, "-o", 2) == 0)	return eOUTPUT;
	return eUNKNOWN;
}

void retreive_arg(char **argv, char **str, int *i)
{
	if (strlen(argv[*i]) > 2)
		*str = &argv[*i][2];
	else {
		*str = argv[*i + 1];
		(*i)++;
	}
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
			case eKEY: { if (i + 1 == argc && strlen(argv[i]) < 2) { free(args); print_error("-k: expected key as next argument"); return NULL; } else { retreive_arg(argv, &args->key, &i); } break; }
			/* -h --help */
			case eHELP: { help(); full_help(); free(args); return NULL; break; }
			/* -v --verbose */
			case eVERBOSE: { args->verbose = 1; break; }
			/* -o <key> */
			case eOUTPUT: { if (i + 1 == argc && strlen(argv[i]) < 2) { free(args); print_error("-o: expected output file as next argument"); return NULL; } else { retreive_arg(argv, &args->out_exe, &i); } break; }
			/* binary or unknown argument */
			default: { if (i == argc - 1) { args->exe_name = argv[argc - 1]; } else { free(args); printf("%s%s: unknown argument\n", ERRORTEMPLATE, argv[i]); return NULL; } break; }
		}
	}

	if (args->verbose) { printf("[+] Using verbose\n"); }

	if (args->exe_name == NULL) {
		print_error("binary not supplied");
		free(args);
		return NULL;
	}
	if (args->verbose) { printf("[+] Using input file `%s`\n", args->exe_name); }

	if (args->out_exe == NULL)
		args->out_exe = DEFAULTOFNAME;
	if (args->verbose) { printf("[+] Using output file `%s`\n", args->out_exe); }

	/* error if input == output */
	if (strcmp(args->exe_name, args->out_exe) == 0) {
		printf("%s%s`%s`%s\n", ERRORTEMPLATE, "input file must be different from output file (using ", DEFAULTOFNAME, " as output file by default)");
		free(args);
		return NULL;
	}

	if (args->key == NULL) {
		if (args->verbose) { printf("[*] No key supplied, generating...\n"); }
		char random_buffer[32];
		if (syscall(SYS_getrandom, random_buffer, 32, GRND_NONBLOCK) == -1) {
			print_error(NULL);
			free(args);
			return NULL;
		}
		for (int i = 0; i < 32; i++) // convert urand0m 32 char to a 64 hex (can probably be done without a for but idk)
			sprintf(&args->random_key[i * 2], "%02X", (unsigned char)random_buffer[i]);
		args->key = args->random_key;
	}
	printf("[+] key_value: %s\n", args->key);

	return args;
}