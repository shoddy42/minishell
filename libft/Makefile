# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/11/04 22:17:35 by wkonings      #+#    #+#                  #
#    Updated: 2022/11/24 19:19:49 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# --------------- VARIABLES ---------------#

NAME = libft.a
CC = gcc
FLAGS = -Wall -Wextra -Werror

# --------------- DIRECTORIES ------------ #

INCLUDE_DIR	:=	include
LIBS_DIR	:=	libft
OBJS_DIR	:= 	obj
SRC_DIR		:=	src

# --------------- FILES -------------------#

HEADERS = libft.h
FILES = tolower toupper isprint isascii isalnum isdigit isalpha atoi \
		strncmp strnstr strrchr strchr strlcat strlcpy strlen memcmp \
		memchr memmove memccpy memcpy bzero memset calloc strdup \
		substr strjoin strtrim split itoa strmapi putchar_fd \
		putstr_fd putendl_fd putnbr_fd lstnew lstadd_front lstsize \
		lstlast lstadd_back lstdelone lstclear lstiter lstmap strcmp \
		charinstr strexpand strisnum strchr_num strclean isspace \
		strcontains

SRCS		:=	$(addprefix $(SRC_DIR)/,$(FILES:%=ft_%.c))
OBJS		:=	$(addprefix $(OBJS_DIR)/,$(notdir $(SRCS:%.c=%.o)))


all: $(NAME)

$(NAME): $(OBJS)
	@echo "\n"
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

$(OBJS_DIR):
	@mkdir -p $@

$(OBJS_DIR)/%.o:$(SRC_DIR)/%.c $(HEADERS) | $(OBJS_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	/bin/rm -rf $(OBJS_DIR)

fclean:	clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
