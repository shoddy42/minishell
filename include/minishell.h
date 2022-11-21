/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/19 17:39:59 by root          ########   odam.nl         */
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
# define EMOJI "💤☢️🐧🤖🧙‍♂️🧙🐉🐲🔥💀🐢🐢🐢☢️⚜️➜ "
# define TITLEA "\x1B[103m\x1B[37;44m ⚜️  🐢 \x1B[0;34;40m shell 𝒔𝒉𝒆𝒍𝒍 \x1B[0m\x1B[92m ᗌ >"
# define TITLEC "\x1B[42mGreen Background\x1B[0m"
# define TITLEB "\x1B[49m\x1B[92m ❱ \x1B[0m"
# define FONT1 "𝓶𝓲𝓷𝓲𝓼𝓱𝓮𝓵𝓵"
# define FONT2 "🐌 🔥 𝓂𝒾𝓃𝒾𝓈𝒽𝑒𝓁𝓁 🔥 🐌"
# define FONT3 "🍉 ⋆ 🍧 🔥 𝓂𝒾𝓃𝒾𝓈𝒽𝑒𝓁𝓁 🔥 🍧 ⋆ 🍉"
# define PROMPT "ᗌ ❱ ▷ ᕗ ᑀ 〉》"

// printf '\e[31m██ = #FF0000\e[m\n'
// # include <sys/wait.h> // needed for WSL

// should ; be token? are tabs actually getting set to void?
# define DELIMITER " |$<>=;\t\'\"\n"

// typedef enum e_from
// {
// 	MINISHELL,
// 	CHILD
// }	t_from;

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
	t_token		*tokens;	//rename to head?
	t_command	*commands;	//rename to head?
	t_env		*env;		//rename to head?
	char		**envp;			//repurpose after init.
	char		**ms_environment;
	char		**path;			//probably dont store this.
	char		*command;
	char		*bin_dir;		//check requirement
	DIR			*bin;

	pid_t		last_cmd;
	int			last_return;	//rename
	int			pipe_count;
	int			hd_count;
	bool		cancel_command; //change to bool
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
void	free_tokens_til(t_token *start, t_token *end, t_minishell *shell);
void	free_single_token(t_token *token);
void	free_tokens(t_minishell *shell);
t_token	*get_last_token(t_token *list);
int		ms_strchr(const char *src, int c);
t_token	*merge_tokens(t_token *start, t_token *end, t_minishell *shell);

// env
void	init_env(t_minishell *shell, char **env);
char	*ms_getenv(char *key, t_minishell *shell);
int		fill_key(t_env	*new);
int		fill_data(t_env	*new, int eq);
void	new_env(char *data, t_minishell *shell);
bool	ms_replace_env(char *beans, t_minishell *shell);
bool	legal_env(char *data);
bool	replace_env(char *beans, t_env *env, t_minishell *shell);
t_env	*env_exists(char	*beans, t_minishell *shell);
void	print_export(t_minishell	*shell);

// execute.c
void	execute(t_command *cmd, t_minishell *shell);
void	execute_two_electric_boogaloo(t_minishell *shell);
// char	**get_command_options(t_token	*token);
t_token	*get_command_options(t_token *token, t_command *cmd);
// char	*pipex_pathjoin(char const *path, char const *cmd);
// ^^^^doesn't need to be in here probably.

// Builtins.c
bool	is_builtin(t_command *cmd, t_minishell *shell);
int		ms_cd(t_command	*cmd, t_minishell *shell);
int		ms_echo(t_command *cmd);
int		ms_pwd(t_command *cmd);
int		print_env(t_minishell	*shell, t_command	*cmd);
int		ms_export(t_command *cmd, t_minishell	*shell);
int		ms_unset(t_minishell *shell, t_command *cmd);

// heredoc.c
t_token	*heredoc(t_token *token, t_minishell *shell);
void	heredoc_loop(t_token *token, t_minishell *shell);
void	ms_heredoc(t_token *token, t_minishell *shell);
void	delete_heredocs(t_minishell *shell);

// init.c
int		init_minishell(t_minishell *shell, char **envp);
void	sighandler(int signum);
void	child_sig(int signum);
void	init_env(t_minishell *shell, char **envp);

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
void    print_envp(char **envp);


// oblivion of main.c
void	parse_append(t_minishell *shell);
#endif