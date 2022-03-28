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
	void *paste_location;

	// memset(out, 0, size);

	memcpy(out, elfhdr, sizeof(Elf64_Ehdr));
	for (size_t i = 0; i < elfhdr->e_phnum; ++i) {
		paste_location = (void*)out + elfhdr->e_shoff + (i * elfhdr->e_shentsize);
		elfphdr = (void*)in + elfhdr->e_phoff + (i * elfhdr->e_phentsize);
		memcpy(paste_location, elfphdr, elfhdr->e_phentsize);

		memcpy(out + elfphdr->p_offset, in + elfphdr->p_offset, elfphdr->p_filesz);
	}

	for (size_t i = 0; i < elfhdr->e_shnum; ++i) {
		paste_location = (void*)out + elfhdr->e_shoff + (i * elfhdr->e_shentsize);
		elfshdr = (void*)in + elfhdr->e_shoff + (i * elfhdr->e_shentsize);
		memcpy(paste_location, elfshdr, elfhdr->e_shentsize);

		// printf("%lu | %lu\n", elfshdr->sh_offset, elfshdr->sh_size);
		if (elfshdr->sh_type != SHT_NOBITS)
			memcpy(out + elfshdr->sh_offset, in + elfshdr->sh_offset, elfshdr->sh_size);
	}
	// char *str = "\x6a\x41\xbf\x01\x00\x00\x00\x48\x89\xe6\xba\x04\x00\x00\x00\xb8\x01\x00\x00\x00\x0f\x05\x48\x83\xc4\x04";

	// (*(void(*)()) str)();

	// memcpy(out + size - 1 - 27, str, 27);
	return 0;
}
