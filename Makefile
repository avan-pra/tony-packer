NAME = tony-packer

CC = clang

CFLAGS = -I$(HDIR)

SRCS =	main.c\
		utils.c\
		argparse.c\
		help.c\
		tony.c\

HDIR = head/

OBJS = $(addprefix srcs/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re: fclean all
