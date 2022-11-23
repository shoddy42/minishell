/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 00:37:37 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//LATER: check if we have any headers we dont need.

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>  //mby?
# include <readline/readline.h> //required
# include <readline/history.h> //required
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "../libft/libft.h"
# include <stdbool.h>
# include <limits.h>
# include <dirent.h>
# include <termios.h>
#define TEST "\x1b"
// choose between 33(blue) and 32(blue) 69 (more turq)
# define PRMT "shell > "
# define YELLOW
# define PRMT_FNCY "\x1b 1;5H\001\x1b[48;5;220m\002 \001 🐢 \002 \001\x1b[0m\x1b[48;5;33;30;1m\002 shell \001\x1b[0m\002 \001\x1b[92m>\002 \001\x1b[0m\002"
# define PRMT_FNCY2 "\e[33m 🐢 \e[0m\e[30;1m shell \e[0m \e[33m> \e[0m\0\0\0\0\0"
# define PRMT_FNCY3 "🐢 shell > \0\0\0\0\0"
# define OK "\001\x1b[103;30m\x02 🐢 "
# define TWO "\x01\x1b[104m\x02 minishell "
# define RESET "\001\033[0m\002 > \003"
# define TITLEA "\001\033[103;30m\002 🐢 \001\x1b[104m\002"
# define TITLEB " MongolShell \001\x1b[49m\x1b[92m❱\002 \001\x1b[0m\002"
# define STOLENTITLEA "\001\x1b[103m\x1b[30m\002 🐎 \001\x1b[104m\002"
# define STOLENTITLEB " MongolShell \001\x1b[49m\x1b[92m❱\002 \001\x1b[0m\002"

# define AT "\1\x1b[103m\x1b[30m\2 🐢 \1\x1b[48;5;33;30m\2"
# define BT " TurtleShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define TURTLE1 "\1\x1b[48;5;220m\x1b[30m\2 🐢 \1\x1b[104m\2"
# define TURTLE2 " TurtleShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m \2\3"

# define HELL1 "\1\x1b[48;5;220m\x1b[30m\2 👹 \1\x1b[101m\2"
# define HELL2 " Minis hell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define DRAGON1 "\1\x1b[48;5;220m\x1b[30m\2 🐉 \1\x1b[38;5;232;48;5;47m\2"
# define DRAGON2 " DragonShell \1\x1b[49m\x1b[92m❱\2\1\x1b[0m\2\3"

# define SPOOKY1 "\1\x1b[48;5;202m\x1b[30m\2 🎃 \1\x1b[38;5;202;48;5;232m\2"
# define SPOOKY2 " SpookyShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define MOON1 "\1\x1b[48;5;234m\x1b[30m\2 🌙 \1\x1b[38;5;252;48;5;234m\2"
# define MOON2 " MoonShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define WIZARD1 "\1\x1b[48;5;45m\x1b[30m\2 🍉 \1\x1b[38;5;232;48;5;200m\2"
# define WIZARD2 " MelonShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define HD1 "\1\x1b[48;5;220m\x1b[30m\2 📄 \1\x1b[48;5;33;30m\2"
# define HD2 " heredoc \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

// ERROR DEFINES

# define SYN_NODELIM "Syntax error near '<<'; No valid DELIM for heredoc. ["

// printf '\e[31m██ = #FF0000\e[m\n'
// # include <sys/wait.h> // needed for WSL

// should ; be token? are tabs actually getting set to void?
# define DELIM " |$<>=;\t\'\"\n"

typedef enum e_pipe
{
	READ = 0,
	WRITE = 1,
	NEEDS_PIPE = -42
}	t_pipe;

//LATER: remove all the numbers
typedef enum e_tokentype
{
	COMMAND = 42,
	QUOTE = 45,
	DQUOTE = 46,
	VARIABLE = 47,
	AND = 48,
	LEFT = 49,
	RIGHT = 50,
	PIPE = 51,
	HEREDOC = 53,
	SEMICOLON = 60,
	OUTFILE = 55,
	INFILE = 56,
	ERROR = 57,
	VOID = 69,
}	t_tokentype;

typedef struct s_token
{
	char			*data;
	int				fd;
	t_tokentype		type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

//LATER: examine whether we need ALL of these
typedef struct s_command
{
	char				**command;	
	bool				executable;
	char				*in_name;
	char				*out_name;
	int					outfile;
	int					infile;
	int					tunnel[2];
	pid_t				pid;
	struct s_command	*next;
	struct s_command	*prev;	
}	t_command;

typedef struct s_env
{
	bool			has_key;
	char			*key;
	char			*beans;
	char			*data;
	struct s_env	*next;
}	t_env;

typedef struct s_shell_data
{
	t_token		*token_head;
	t_command	*cmd_head;
	t_env		*env_head;
	char		**envp;
	char		**path;
	char		*command;
	size_t		command_len;
	char		*bin_dir;		//check requirement
	DIR			*bin;

	bool		hd_history;
	int			prompt_type;
	char		*prompt;
	pid_t		last_cmd;
	// int			last_return;	//rename
	int			pipe_count;
	int			hd_count;
	bool		cancel_command;
	bool		cancel_all_commands;
	int			exit;
}	t_minishell;

// tokenize.c
void	set_token_type(t_minishell *shell, t_token *token, char *data);
t_token	*new_token(t_minishell *shell, char *data, int len, bool link);
void	ft_tokenize(t_minishell *shell, char *command);

// token_utils.c
void	print_tokens(t_minishell *shell); //REMOVE LATER
void	print_tokens_backwards(t_minishell *shell); //REMOVE LATER
char	*print_token_type(int type); // REMOVE LATER
void	remove_tokens(t_token *start, t_token *end, t_minishell *shell);
void	free_single_token(t_token *token);
void	free_tokens(t_minishell *shell);
t_token	*get_last_token(t_token *list);
int		ms_strchr(const char *src, int c);
t_token	*merge_tokens(t_token *start, t_token *end, t_minishell *shell);

// env
void	init_env(t_minishell *shell, char **env);
char	*ms_getenv(char *key, t_minishell *shell);
int		fill_env(t_env	*new);
int		new_env(char *data, t_minishell *shell);
bool	ms_replace_env(char *beans, t_minishell *shell);
bool	legal_env(char *data);
int		replace_env(char *beans, t_env *env, t_minishell *shell);
t_env	*env_exists(char	*beans, t_minishell *shell);
void	print_export(t_minishell	*shell);

// envp.c
void	create_envp(t_minishell *shell);

// execute.c
void	execute(t_command *cmd, t_minishell *shell);
void	execute_two_electric_boogaloo(t_minishell *shell);
// char	**get_command_options(t_token	*token);
t_token	*get_command_options(t_token *token, t_command *cmd);
char	*pipex_pathjoin(char const *path, char const *cmd);
// ^^^^doesn't need to be in here probably.
// execute_utils.c
void	parent_close(t_command *cmd, t_minishell *shell);
char	*pipex_pathjoin(char const *path, char const *cmd);
bool	handle_path(char **args, char **envp);
void	local_command(char **args, char **envp);
void	set_status(t_minishell *shell, int cmd_count);
// Builtins.c
bool	is_builtin(t_command *cmd, t_minishell *shell);
int		ms_cd(t_command	*cmd, t_minishell *shell);
int		ms_echo(t_command *cmd);
int		ms_pwd(t_command *cmd);
int		print_env(t_minishell	*shell, t_command	*cmd);
int		ms_export(t_command *cmd, t_minishell	*shell);
int		ms_unset(t_minishell *shell, t_command *cmd);
int		close_builtin(t_command *cmd, t_minishell *shell);

// heredoc.c
t_token	*heredoc(t_token *token, t_minishell *shell);
void	heredoc_loop(t_token *token, t_minishell *shell);
void	ms_heredoc(t_token *token, t_minishell *shell);
void	delete_heredocs(t_minishell *shell);

// heredoc_utils.c
char	*get_hd_name(t_minishell	*shell);
void	delete_heredocs(t_minishell *shell);
int		hd_var(int fd, char *line, t_minishell *shell);
void	heredoc_sig(int signum);

// init.c
int		init_minishell(t_minishell *shell, char **envp);
void	sighandler(int signum);
void	child_sig(int signum);
void	init_env(t_minishell *shell, char **envp);
void	create_bin(t_minishell *shell);

// redirects.c
t_token	*handle_left(t_token *token, t_minishell *shell);
t_token	*handle_right(t_token *token, t_minishell *shell);

// quote.c
t_token	*handle_quote(t_token *token, int type, t_minishell *shell);

// expand.c
void	expand_dong(t_token *token, t_minishell *shell);
t_token	*expand_two(t_token *token, t_minishell *shell);

// error.c 
void	ms_error(char *msg, int code, bool terminate, t_minishell *shell);
t_token	*token_error(t_token *token, char *msg, bool print_token);

// commands.c
int		make_commands(t_minishell *shell);
void	print_commands(t_minishell *shell); //remove later

//env_utils.c
void	create_envp(t_minishell *shell);
// char	**create_envp(t_env *env_head);
void	print_envp(char **envp);

// oblivion of main.c
void	parse_append(t_minishell *shell);
#endif