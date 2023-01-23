# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings      #+#    #+#                  #
#    Updated: 2023/01/23 02:58:54 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------- #
# --------------- VARIABLES --------------- #
# ----------------------------------------- #

NAME = minishell
FLAGS = -Wall -Wextra -Werror
DEBUG = #-g #-fsanitize=address
MAKEFILE = makefile
# CC = clang

# ---------------------------------------- #
# --------------- DIRECTORIES ------------ #d
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

BIN_DIR		:= bin
INCLUDE_DIR	:= include
LIB_DIR		:= libft

INCLUDES	:= libft/libft.a
HEADERS_DIR	:= include
INC			:= -I include

# ----------------------------------------- #
# --------------- FILES ------------------- #
# ----------------------------------------- #

HEADER_FILES:= minishell.h prompt.h
HEADERS		:=	$(addprefix $(INCLUDE_DIR)/, $(HEADER_FILES))

MAIN_FILES	:= main error init signals cleanup
TOKEN_FILES := tokenize token_utils
ENV_FILES	:= env env_utils unset export envp
EXEC_FILES	:= commands execute execute_utils
BUILT_FILES	:= builtins cd pwd echo prompt
PARSE_FILES	:= parser quote expand redirects heredoc heredoc_utils

FILES	:=	$(MAIN_FILES:%=%.c) \
			$(addprefix $(S_TOKEN)/, $(TOKEN_FILES:%=%.c)) \
			$(addprefix $(S_PARSE)/, $(PARSE_FILES:%=%.c)) \
			$(addprefix $(S_EXEC)/, $(EXEC_FILES:%=%.c)) \
			$(addprefix $(S_ENV)/, $(ENV_FILES:%=%.c)) \
			$(addprefix $(S_BUILTIN)/, $(BUILT_FILES:%=%.c))

LIBS	:=	libft
SRCS	:=	$(addprefix $(SRC_DIR)/, $(FILES))
OBJS	:=	$(addprefix $(OBJ_DIR)/, $(FILES:%.c=%.o))

LIBFT := libft
LIBFT_A	:= $(LIBFT)/libft.a

# ----------------------------------------- #
# --------------- BREW -------------------- #
# ----------------------------------------- #

BREW_DIR	= $(shell brew --prefix)
LIB_READLINE	= $(BREW_DIR)/opt/readline/lib
INCLUDE_READLINE = -I $(BREW_DIR)/opt/readline/include
READLINE_DIRS = -L $(LIB_READLINE) $(READLINE)

# ----------------------------------------- #
# --------- COLOURS / FORMATTING ---------- #
# ----------------------------------------- #

#Formatting
RESET	:= \1\33[0m\2
END		:= \1\33[0m\2\3
BANNER	:= banner

#Colours
YELLOW	:= \1\33[38;5;220m\2
BLACK	:= \1\33[38;5;232m\2
ORANGE	:= \1\33[38;5;202m\2
RED		:= \1\33[38;5;196m\2
PINK	:= \1\33[38;5;198m\2
GREEN	:= \1\33[38;5;28m\2
BLUE	:= \1\33[38;5;33m\2
L_BLUE	:= \1\33[38;5;69m\2
D_BLUE	:= \1\33[38;5;21m\2
GREY	:= \1\33[38;5;242m\2
PURPLE	:= \1\33[38;5;92m\2
VIOLET	:= \1\33[38;5;183m\2

# ----------------------------------------- #
# --------------- RECIPES ----------------- #
# ----------------------------------------- #

$(NAME): $(LIBFT_A) $(OBJS) $(HEADERS) $(MAKEFILE) | $(BIN_DIR)
	@printf "$(BLUE)Compiling $(YELLOW)$(NAME).\n$(END)"
	@$(CC) $(FLAGS) $(DEBUG) $(SRCS) -o $(NAME) -I include $(INCLUDES) -lreadline $(READLINE_DIRS) $(INCLUDE_READLINE)
	@make hell
	@printf "$(YELLOW)minihell compiled. $(RED)continue at your own risk.\n$(END)"

all: $(NAME)

$(LIBFT_A): $(LIBFT)
	@printf "test\n"
	@make -C $(LIB_DIR)

$(LIBFT):
	@git clone https://github.com/shoddy42/libft

$(BIN_DIR):
	@printf "$(YELLOW)Creating $(RESET)/$@/ directory.\n$(END)"
	@mkdir -p $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(BANNER)
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)Compiling $(PINK)$(notdir $@) $(RESET)from $(RED)$(notdir $<)$(END)\n"
	@$(CC) $(FLAGS) -I include $(INCLUDE_READLINE) -c $< -o $@

clean:
	@printf "$(YELLOW)Cleaning up $(RESET)$(NAME)!\n$(END)"
	/bin/rm -rf $(OBJ_DIR)

fclean:	clean
	/bin/rm -f $(NAME)
	/bin/rm -rf $(BIN_DIR)
	@make fclean -C $(LIBFT)
	@printf "$(L_BLUE)Fully cleaned $(RESET)$(NAME)\n$(END)"

tooclean: fclean
	/bin/rm -rf $(LIBFT)

re: fclean all

tooreal: tooclean all

test: $(NAME)
	@./$(NAME)

pog:
	@printf "\n$(RED)"
	@printf "⣿⣿⣿⣿⡿⠟⠛⠛⠛⠛⠉⠉⠙⠛⠛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠟   \n"
	@printf "⣿⣿⣯⣥⣤⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⣿⣏⣀⣀⣀⡀  \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠻⠿⠟⠉⠉⠉⢻⣿⣿⣿⡿⠟⠋⣡⣼⣿⣿⣿⡄ \n"
	@printf "⣿⣿⣿⣟⣭⣤⣶⣶⣿⣿⠃⠀⠀⢀⣀⣤⣶⣿⣿⣿⣿⡅⡀⢀⣩⣤⣤    \n"
	@printf "⣿⣿⣿⣿⣿⣿⣛⡛⠛⠛⠛⢋⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⠛⠛⠓⠠   \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣤⣦  \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇ \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⠿⢿⡿⢿⣿⣿⣿⠃ \n"
	@printf "⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣥⣄⣀⣀⠀⠀⠀⠀⠀⢰⣾⣿⣿⠏  \n"
	@printf "⠀⠀⠀⠀⠀⠀⠉⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣜⡻⠋    \n"
	@printf "⣰⣾⣷⣶⣿⣾⣖⣻⣿⣿⡿⣿⣿⣿⣿⠿⠿⠟⠛⠛⠛⠋⠉⠉⢉⡽⠃    \n"
	@printf "⣿⣿⣿⣿⣿⣿⡉⠉⠉⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⡤⠚⠉      \n"
	@printf "⠛⠛⣿⣿⣿⣿⣿⣿⣿⠉⠛⢶⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⡇        \n"
	@printf "⠠⣾⣿⣿⣿⣿⣿⠿⠟⠃⠀⠀⠀⠈⠲⣴⣦⣤⣤⣤⣶⡾⠁        \n"
	@printf "⠄⠈⠉⠻⢿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉          \n"
	@printf "$(END)"

hell:
	@printf "\n$(BLACK)"
	@printf "            WARNING: ENTERING MINI'S HELL\n"
	@printf "⠈⣾⣿⣿⣿⣿⣿⡿⣟⡿⣹⢯⡽⣭⢯⣝⡮⣝⢻⢿⡖⡰⣀⠆⡩⡘⢩⡉⢭⡑⣍⠲⡱⣎⢟⡻⣛⢟⡻⣟⠿⣿⢿⣿⣿⣿⣿⣿⣿⠂\n"
	@printf "⠰⣿⣿⣿⣿⣿⢯⣟⣵⡻⣧⣟⣾⣱⡟⣮⡽⢯⣿⣞⣷⡱⣌⢎⡱⣌⢣⡜⢦⢓⡬⣳⣝⣮⢳⡳⣝⢮⣓⢮⡽⢬⡳⢭⣻⣿⣿⣿⣿⠁\n"
	@printf "⢾⣿⣿⣿⣿⢯⣟⣾⣳⣿⢟⠿⣛⠻⢿⣷⣿⣻⣞⣿⣿⣷⡝⡮⢵⣊⠷⣸⢧⣻⣼⣷⣟⢾⡳⣝⢮⡲⣉⠞⡸⢧⣛⣬⢳⡽⣿⣿⣿⡄\n"
	@printf "⢘⣿⡿⣽⣯⣟⣾⣳⣿⣿⣾⣿⣾⣷⣶⣬⡿⣿⣾⣳⡟⣿⢽⣙⠦⡝⢺⣵⡻⢯⡿⣝⢮⡷⣿⣾⣿⣥⣧⣮⣱⣋⢾⣜⣧⢻⡽⣿⣿⣾\n"
	@printf "⢈⣿⢿⡽⣾⡽⣾⣿⣿⡟⠉⣿⣇⣤⣈⡝⠿⣿⣿⣷⡹⣏⠾⡑⢊⠐⡁⠊⠽⠳⣽⢮⣷⣿⣿⠿⢛⠉⣙⠛⡻⢿⣿⣾⣷⣫⣽⣳⣿⣧\n"
	@printf "⣼⡿⣯⣟⢷⡻⣿⣿⣿⣆⢼⣿⣿⣿⣿⡿⢀⣻⣿⣷⠱⣊⠧⡑⠬⡐⠤⡑⠨⡑⠨⣉⣿⣿⠃⣸⣿⣷⣿⣿⣷⢈⣿⣿⣷⢣⡗⣯⣻⡇\n"
	@printf "⣾⣿⣟⣾⢣⢟⡱⣿⣿⣿⣞⣿⣿⣿⣿⣏⣷⡿⣿⢇⠳⣌⢳⡉⠦⡑⢆⠱⢳⣌⠡⠼⢏⠻⣷⣸⣿⣿⣿⡿⢏⣢⣿⣿⢇⠯⣜⣳⣿⡇\n"
	@printf "⣿⣿⡾⣝⣏⢎⡱⢍⡻⠿⣿⢿⡿⣿⠟⠿⣉⠖⣡⢎⡳⢬⡓⣬⢑⠰⡈⠜⠳⢿⣧⡘⢠⢁⠛⠿⣷⣿⣾⣿⣾⣿⠟⡫⢌⠲⣩⢷⣻⣧\n"
	@printf "⣿⣿⡽⣏⠾⣌⠲⣌⠰⣉⠖⢣⠓⡌⢎⡱⢌⡚⣴⢫⡜⣣⠝⡤⢃⠆⡘⠌⠳⡌⢿⣷⡄⠊⢌⠒⢤⠉⡜⣉⠱⢈⢂⢁⠢⡑⢧⣻⣿⣧\n"
	@printf "⣿⣿⣽⢯⡟⣬⠳⣄⠳⢄⢪⠡⡙⡜⢬⡑⢎⢴⣫⠳⣜⡱⢎⠰⣁⠂⠤⣉⠒⡸⢌⣻⣷⡈⠄⡉⠆⡑⠐⡠⢁⠆⠌⡄⢣⢜⣣⢟⣿⣿\n"
	@printf "⣿⣿⣾⢯⡿⣜⡳⣌⠳⣈⢆⠳⡘⡜⡰⡘⢬⢾⣟⠳⣌⠳⢎⢡⠂⠌⠐⠠⢃⠦⡑⠤⣿⣧⠀⠡⠊⢄⠃⡄⢃⠌⡒⢌⠶⣩⢞⣯⢯⣿\n"
	@printf "⣿⣿⣾⡿⣽⣳⣝⣎⡳⣡⢎⡱⢱⢌⡱⣉⢖⣯⣌⠓⡌⡑⡊⢄⢲⣀⡄⡀⢀⠂⢍⡘⠌⣿⡆⠡⢈⠢⡑⢄⠣⡘⡜⣬⢳⡝⣾⣽⣻⣏\n"
	@printf "⣿⣿⣯⣿⣟⡷⣯⣞⡵⣣⢎⡕⣣⠎⡴⣘⣾⣿⣿⣷⣼⣵⣧⢎⡔⣛⠛⣛⠢⡜⣤⣶⣽⣾⣷⢀⠂⡱⢈⠦⢱⡑⣞⡴⣏⣿⣳⣯⡇⣿\n"
	@printf "⣿⣿⣿⣟⣿⣿⣳⡿⣜⢧⡞⡜⢦⡹⢰⡡⣿⣿⣻⣿⣿⣿⣿⣯⣾⣴⣫⣖⣳⣽⣿⣿⣿⣿⣇⠂⡘⢄⠣⢎⡱⠼⣜⣳⣟⣾⡷⣿⠘⡿\n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⡽⣮⢳⡝⡶⣩⢇⠖⡹⢿⣧⡜⠛⠿⢿⣿⣿⣿⣿⣿⠿⠿⠿⡉⢾⡿⢁⠢⡘⢤⢋⠶⣩⢟⣞⡷⣯⣷⣿⣟⠀⣿\n"
	@printf "⠙⣿⡿⣿⣿⣿⣿⣿⣿⣯⢷⣝⢶⢣⢮⡹⢔⠣⣉⢒⡉⢆⡱⢊⠭⢩⠙⡍⢢⢁⢂⠑⡂⠰⡈⢆⡱⢌⢎⡳⣝⣾⡽⣿⣟⣿⠿⡍⠐⡋\n"
	@printf "⠀⠘⣿⣛⣿⣿⣿⣿⣿⣿⣿⣞⣯⢻⡼⡱⢎⡵⡐⢎⡐⢢⠡⡉⠰⡁⢎⡐⢂⠌⣀⠃⡌⠡⢜⠢⡕⣎⢮⣳⣟⣾⣿⣟⡯⠃⠀⠀⠀⠃\n"
	@printf "⠀⠀⠈⠙⠛⣿⣿⣿⣿⣿⣿⣿⣿⣻⣼⣹⣚⡴⣙⢦⡙⢤⠓⡌⠁⡌⠐⡌⢡⢊⡄⢣⡌⢳⡌⢳⡹⣼⣳⣿⢯⣿⡟⢈⠄⠀⠀⠀⠀⠀\n"
	@printf "⠀⠀⠀⠀⠀⣿⣛⣿⣿⣿⣿⣿⣿⣿⣷⣧⢿⣼⣻⡾⡝⢢⢣⠐⡡⠌⢡⠘⡐⠦⡘⢷⣜⣧⣚⣵⣻⣿⢿⣽⡿⢋⣴⡿⠀⠀⠀⠀⠀⠀\n"
	@printf "⠀⠀⠀⠀⠀⣿⢣⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣏⠷⣌⠡⢂⠍⡐⠌⢂⠡⠑⢂⡉⢎⡽⢿⣾⣿⣿⣟⣿⢯⣃⣿⠋⠀⠀⠀⠀⠀⠀⠀\n"
	@printf "⠀⠀⠀⠀⠀⢺⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣬⣳⣣⣒⠴⣈⡆⢦⣑⣦⣹⣼⣜⣿⣿⣿⣷⡿⣿⢿⡏⠁⠀⠀⠀⠀⠀⠀⠀⠀\n"
	@printf "⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣷⣿⣿⣼⡏⠁⠀⠀⠀⠀⠀⠀⠀⠀\n"
	@printf "⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢯⣿⣾⣅⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	@printf "⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣟⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣷⢻⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	@printf "⠀⠀⠀⠀⠀⠀⠀⣼⣯⣿⣷⣿⢿⣻⣿⣿⣟⡾⣹⢿⣿⣷⣻⣞⢷⣻⣽⣿⣿⢳⢯⣟⡿⣿⣽⣷⢿⣿⣡⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	@printf "$(END)"

banner:
	@printf " $(YELLOW)  ___  ____       _    $(RED) _          _ _ \n"
	@printf " $(YELLOW)  |  \/  (_)     (_)   $(RED)| |        | | |\n"
	@printf " $(YELLOW)  | .  . |_ _ __  _ ___$(RED)| |__   ___| | |\n"
	@printf " $(YELLOW)  | |\/| | | '_ \| / __$(RED)| '_ \ / _ \ | |\n"
	@printf " $(YELLOW)  | |  | | | | | | \__ $(RED)\ | | |  __/ | |\n"
	@printf " $(YELLOW)  \_|  |_/_|_| |_|_|___$(RED)/_| |_|\___|_|_|\n"
	@printf "$(END)"

.PHONY: all clean fclean tooclean re tooreal test pog hell banner