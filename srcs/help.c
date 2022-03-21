#include <stdio.h>

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
