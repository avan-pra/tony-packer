#include <stdio.h>

int help()
{
	printf("Usage: `tony-packer [options] <binary>` (-h for help)\n");
	return 1;
}

int full_help()
{
	printf("\n-h / --help				| show help\n\
-k <key> / --key <key>			| use custom key \n\
-o <file> / --output <file>		| specify output file\n\
-v / --verbose				| verbose output\n");
	return 1;
}
