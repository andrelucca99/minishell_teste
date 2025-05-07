NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = src/main.c src/builtins.c src/parser.c src/executor.c src/lexer.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft
LDFLAGS = -L$(LIBFT) -lft -lreadline

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

$(LIBFT):
	make -C $(LIBFT)

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT) clean

re: fclean all

.PHONY: all clean fclean re libft
