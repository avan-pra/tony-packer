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
#include <elf.h>
#include "args.h"
#include "utils.h"

int pack_elf(unsigned char *in, unsigned char *out, size_t size, t_args *args)
{
	Elf64_Ehdr *elfhdr = (void*)in;
	Elf64_Phdr *elfphdr = (void*)in + elfhdr->e_phoff;
	Elf64_Shdr *elfshdr = (void*)in + elfhdr->e_shoff;

	return 0;
}
