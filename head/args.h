#ifndef ARGS_H
# define ARGS_H

#define ERRORTEMPLATE "Error: "
#define DEFAULTOFNAME "a.out"

typedef struct s_args
{
	char			*exe_name;
	char			*out_exe;
	char			*key;
	int				verbose;
	char	random_key[65];
}				t_args;

t_args *getargs(int argc, char **argv);

#endif

