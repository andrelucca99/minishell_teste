NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = src/main.c src/sigint.c src/builtins/builtins.c src/parser/parser.c src/parser/utils.c src/executor/executor.c src/executor/utils.c src/tokens.c src/builtins/builtin_echo.c src/builtins/builtin_pwd.c src/builtins/builtin_env.c src/builtins/builtin_cd.c src/builtins/builtin_exit.c
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
