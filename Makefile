NAME = tony-paker

CC = clang

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c

OBJS = $(addprefix srcs/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re: fclean all
