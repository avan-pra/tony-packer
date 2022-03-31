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

Elf64_Phdr *get_text_segment(const unsigned char *in)
{
	Elf64_Phdr *elfphdr;

	for (size_t i = 0; i < ((Elf64_Ehdr*)in)->e_phnum; ++i)
	{
		elfphdr = (void*)in + ((Elf64_Ehdr*)in)->e_phoff + (i * ((Elf64_Ehdr*)in)->e_phentsize);
		if (elfphdr->p_type == PT_LOAD && elfphdr->p_flags == (PF_R | PF_X))
			return elfphdr;
	}
	return NULL;
}

static void copy_elf_header(const unsigned char *in, unsigned char *out)
{
	memcpy(out, in, sizeof(Elf64_Ehdr));
}

static void copy_program(const unsigned char *in, unsigned char *out)
{
	Elf64_Ehdr *elfhdr = (void*)in;
	Elf64_Phdr *elfphdr;
	void *paste_location;

	for (size_t i = 0; i < elfhdr->e_phnum; ++i) {
		paste_location = (void*)out + elfhdr->e_shoff + (i * elfhdr->e_shentsize);
		elfphdr = (void*)in + elfhdr->e_phoff + (i * elfhdr->e_phentsize);
		memcpy(paste_location, elfphdr, elfhdr->e_phentsize);

		memcpy(out + elfphdr->p_offset, in + elfphdr->p_offset, elfphdr->p_filesz);
	}
}

static void copy_section(const unsigned char *in, unsigned char *out)
{
	Elf64_Ehdr *elfhdr = (void*)in;
	Elf64_Shdr *elfshdr;
	void *paste_location;

	for (size_t i = 0; i < elfhdr->e_shnum; ++i) {
		paste_location = (void*)out + elfhdr->e_shoff + (i * elfhdr->e_shentsize);
		elfshdr = (void*)in + elfhdr->e_shoff + (i * elfhdr->e_shentsize);
		memcpy(paste_location, elfshdr, elfhdr->e_shentsize);

		if (elfshdr->sh_type != SHT_NOBITS)
			memcpy(out + elfshdr->sh_offset, in + elfshdr->sh_offset, elfshdr->sh_size);
	}
}

int pack_elf(unsigned char *in, unsigned char *out, size_t size, t_args *args)
{
	Elf64_Ehdr *elfhdr = (void*)in;
	Elf64_Phdr *elfphdr = (void*)in + elfhdr->e_phoff;
	Elf64_Shdr *elfshdr = (void*)in + elfhdr->e_shoff;
	void *paste_location;

	copy_elf_header(in, out);

	copy_program(in, out);

	copy_section(in, out);

	return 0;
}
