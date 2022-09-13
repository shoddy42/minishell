# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings      #+#    #+#                  #
#    Updated: 2022/09/14 01:15:48 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = shell
SOURCES = $(FILES:%=%.c)
OBJECTS = $(FILES:%=%.o)
INCLUDES = libft/libft.a
FLAGS = -Wall -Wextra -Werror
HEADERS = minishell.h
FILES = main tokenize token_utils

# BREW_DIR	= $(shell brew --prefix)
# LIB_READLINE	= $(BREW_DIR)/opt/readline/lib
# INCLUDE_READLINE = -I $(BREW_DIR)/opt/readline/include
# READLINE_DIRS = -L $(LIB_READLINE) $(READLINE)

all: $(NAME)

$(NAME):
	gcc $(SOURCES) $(INCLUDES) $(READLINE_DIRS) -lreadline 

flags:
	gcc $(FLAGS) -c $(SOURCES) -lreadline
	./a.out

test: re
	./a.out
clean:
	/bin/rm -f $(OBJECTS)

fclean:	clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
