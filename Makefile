# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings      #+#    #+#                  #
#    Updated: 2022/11/16 16:15:55 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#todo: make Makefile always call make on library makefile to make sure ITS make is upto date.
#todo: also make sure the header is up to date when constructing .o NEW_FILES i think
# ----------------------------------------- #
# --------------- VARIABLES --------------- #
# ----------------------------------------- #

NAME = minishell
FLAGS = -Wall -Wextra -Werror
DEBUG = -g #-fsanitize=address
CC = gcc

# ---------------------------------------- #
# --------------- DIRECTORIES ------------ #
# ---------------------------------------- #

#src and its subdirectories.
SRC_DIR		:= src
S_ENV		:= env
S_EXEC		:= executor
S_BUILTIN	:= builtins
S_TOKEN		:= tokens
S_PARSE		:= parser

OBJ_DIR		:=	obj
OBJ_BAD		:=	env executor builtins tokens parser
OBJ_SUB		:=	$(addprefix $(OBJ_DIR)/, $(OBJ_BAD))
# O_ENV		:=	env
# O_EXEC		:=	executor
# O_BUILTIN	:=	builtins
# OBJ_DIRS	:=	$(addprefix $(OBJ_DIR)/, (O_ENV))
# 				$(addprefix $(OBJ_DIR)/, (O_EXEC))
# 				$(addprefix $(OBJ_DIR)/, (O_BUILTIN))

BIN_DIR		:= bin
INCLUDE_DIR	:= include
LIB_DIR		:= libft

INCLUDES	:= libft/libft.a
HEADERS_DIR	:= include
INC			:= -I include

# ----------------------------------------- #
# --------------- NEW_FILES ------------------- #
# ----------------------------------------- #

#this cant be fully right?
HEADER_FILES:= $(INCLUDE_DIR)/minishell.h
HEADERS		:=	$(addprefix $(INCLUDE_DIR)/, $(HEADER_FILES))

MAIN_FILES	:= main error init
TOKEN_FILES := tokenize token_utils
ENV_FILES	:= env env_utils unset
EXEC_FILES	:= commands execute
BUILT_FILES	:= builtins cd pwd echo
PARSE_FILES	:= quote expand redirects heredoc

NEW_FILES	:=	$(MAIN_FILES:%=%.c) \
				$(addprefix $(S_TOKEN)/, $(TOKEN_FILES:%=%.c)) \
				$(addprefix $(S_PARSE)/, $(PARSE_FILES:%=%.c)) \
				$(addprefix $(S_EXEC)/, $(EXEC_FILES:%=%.c)) \
				$(addprefix $(S_ENV)/, $(ENV_FILES:%=%.c)) \
				$(addprefix $(S_BUILTIN)/, $(BUILT_FILES:%=%.c))

LIBS		:=	
SRCS		:=	$(addprefix $(SRC_DIR)/,$(NEW_FILES))
OBJS		:=	$(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# ----------------------------------------- #
# --------------- BREW -------------------- #
# ----------------------------------------- #

BREW_DIR	= $(shell brew --prefix)
LIB_READLINE	= $(BREW_DIR)/opt/readline/lib
INCLUDE_READLINE = -I $(BREW_DIR)/opt/readline/include
READLINE_DIRS = -L $(LIB_READLINE) $(READLINE)

# ----------------------------------------- #
# --------------- RECIPES ----------------- #
# ----------------------------------------- #


$(NAME): $(OBJS) $(LIBS) $(HEADER_FILES)
	@make -C $(LIB_DIR)
	@echo COMPILING EXECUTABLE
	@gcc $(DEBUG) $(SRCS) -o $(NAME) -I include/minishell.h $(INCLUDES) -lreadline $(READLINE_DIRS) $(INCLUDE_READLINE)

echo:
	@echo $(SRCS)

all: $(NAME)

$(LIBS):
	@echo making libft
	@make -C $(LIB_DIR)
	@echo "MAKING LIBFT RN YEA"

$(OBJ_SUB):
	@mkdir -p $@

$(OBJ_DIR): | $(OBJ_SUB)
	@mkdir -p $@

# $(OBJS)%.o:
# 	@echo Compiling $@
# $(CC) -I include/minishell.h $(INCLUDE_READLINE) -c $< -o $@
# $(OBJ_DIR)/%.o:$(SRCS) | $(OBJ_DIR)
# 	@echo Creating $<.
# 	@$(CC) -I include/minishell.h $(INCLUDE_READLINE) -c $< -o $@

# $(OBJS): $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
# 	@echo $<

# add flags back into the CC
# $(OBJ_DIR)/%.o:$(SRCS:$(SRC_DIR)%=$(OBJ_DIR)%)/%.c | $(OBJ_DIR)
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo Creating file $@ from $< 
	@$(CC) -I include/minishell.h $(INCLUDE_READLINE) -c $< -o $@
# -lreadline $(READLINE_DIRS) $(INCLUDE_READLINE)

flags:
	gcc $(FLAGS) $(SOURCES) -lft $(INCLUDES) -lreadline $(READLINE_DIRS) $(INCLUDE_READLINE)
	./$(NAME)

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean:	clean
	/bin/rm -f $(NAME)

re: fclean all

test: $(NAME)
	@echo ⣿⣿⣿⣿⡿⠟⠛⠛⠛⠛⠉⠉⠙⠛⠛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠟
	@echo ⣿⣿⣯⣥⣤⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⣿⣏⣀⣀⣀⡀
	@echo ⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠻⠿⠟⠉⠉⠉⢻⣿⣿⣿⡿⠟⠋⣡⣼⣿⣿⣿⡄
	@echo ⣿⣿⣿⣟⣭⣤⣶⣶⣿⣿⠃⠀⠀⢀⣀⣤⣶⣿⣿⣿⣿⡅⡀⢀⣩⣤⣤
	@echo ⣿⣿⣿⣿⣿⣿⣛⡛⠛⠛⠛⢋⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⠛⠛⠓⠠
	@echo ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣤⣦
	@echo ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇
	@echo ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⠿⢿⡿⢿⣿⣿⣿⠃
	@echo ⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣥⣄⣀⣀⠀⠀⠀⠀⠀⢰⣾⣿⣿⠏
	@echo ⠀⠀⠀⠀⠀⠀⠉⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣜⡻⠋
	@echo ⣰⣾⣷⣶⣿⣾⣖⣻⣿⣿⡿⣿⣿⣿⣿⠿⠿⠟⠛⠛⠛⠋⠉⠉⢉⡽⠃
	@echo ⣿⣿⣿⣿⣿⣿⡉⠉⠉⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⡤⠚⠉
	@echo ⠛⠛⣿⣿⣿⣿⣿⣿⣿⠉⠛⢶⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⡇
	@echo ⠠⣾⣿⣿⣿⣿⣿⠿⠟⠃⠀⠀⠀⠈⠲⣴⣦⣤⣤⣤⣶⡾⠁
	@echo ⠄⠈⠉⠻⢿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉
	./$(NAME)

.PHONY: all clean fclean re test
