#ifndef UTILS_H
# define UTILS_H

# include "args.h"

int help();
int full_help();
int print_error(char *str);

int pack(t_args *args);
int pack_elf(unsigned char *in, unsigned char *out, size_t size, t_args *args);

#endif