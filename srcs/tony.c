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

typedef struct all_formats {
	Elf64_Ehdr *elf64;
}			   a_formats;

enum formats
{
	eELF,
	eUNKNOWN
};

enum formats find_file_format(unsigned char *inptr)
{
	struct all_formats af = { (void*)inptr };

	if (strncmp((char *)af.elf64->e_ident, "\177ELF", 4) == 0) { return eELF; }
	return eUNKNOWN;
}

int pack(t_args *args)
{
	int in, of;
	unsigned char *inptr, *ofptr;
	struct stat statbuf; // get exe_name file_size with lseek trick
	int ret = 0;
	size_t size;

	/* setup input file */
	if ((in = open(args->exe_name, O_RDONLY)) == -1) {
		print_error(args->exe_name); free(args);
		return 1;
	}
	syscall(SYS_fstat, in, &statbuf);
	size = statbuf.st_size;
	if (size == 0) {
		print_error("Unknown file format"); free(args); close(in);
		return 1;
	}
	if ((inptr = mmap(NULL, size, PROT_READ, MAP_SHARED, in, 0)) == MAP_FAILED) {
		print_error("mmap"); free(args); close(in);
		return 1;
	}
	close(in);

	/* setup output file */
	if ((of = open(args->out_exe, O_RDWR | O_CREAT, 0744)) == -1) {
		print_error(args->out_exe); free(args); munmap(inptr, size);
		return 1;
	}
	if ((ofptr = mmap(NULL, (size), PROT_READ | PROT_WRITE, MAP_SHARED, of, 0)) == MAP_FAILED) {
		print_error("mmap"); free(args); munmap(inptr, size); close(of);
		return 1;
	}
	lseek(of, size - 1, SEEK_SET);
	write(of, "", 1);
	close(of);

	enum formats f = find_file_format(inptr);
	switch (f) {
		case eELF: { printf("[+] Got file format: ELF\n"); ret = pack_elf(inptr, ofptr, size, args); break; }
		default: { ret = print_error("Unknown file format"); }
	}

	munmap(inptr, size);
	munmap(ofptr, (size));
	free(args);
	return ret;
}
