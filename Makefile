# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings      #+#    #+#                  #
#    Updated: 2022/09/08 16:43:28 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = shell
SOURCES = $(FILES:%=%.c)
OBJECTS = $(FILES:%=%.o)
FLAGS = -Wall -Wextra -Werror
HEADERS = minishell.h
FILES = main

all: $(NAME)

$(NAME):
	gcc $(FLAGS) -c $(SOURCES)

test: re
	gcc main.c -lreadline
	./a.out
clean:
	/bin/rm -f $(OBJECTS) $(BOBJECTS)

fclean:	clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
