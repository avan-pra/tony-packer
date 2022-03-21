#ifndef ARGS_H
# define ARGS_H

#define ERRORTEMPLATE "Error: "

typedef struct s_args
{
	char *exe_name;
	char *key;

}				t_args;

t_args *getargs(int argc, char **argv);

#endif

