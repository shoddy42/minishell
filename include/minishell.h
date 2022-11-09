/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/09 12:18:17 by root          ########   odam.nl         */
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

// # include <sys/wait.h> // needed for WSL

# define DELIMITER " |$<>=;\t\'\"\n"

typedef enum e_from
{
	MINISHELL,
	CHILD
}	t_from;

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
	HEREDOC_FILE = 54,
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
	int					outfile;
	int					infile;
	int					tunnel[2];
	pid_t				pid;
	struct s_command	*next;
	struct s_command	*prev;	
}	t_command;

typedef struct s_env
{
	bool			has_beans;
	char			*key;
	char			*beans;
	char			*data;
	struct s_env	*next;
}	t_env;

typedef struct s_shell_data
{
	t_token		*tokens;
	t_command	*commands;
	t_env		*env;
	char		**envp;
	char		**path;
	char		*command;
	char		*bin_dir;

	pid_t		last_cmd;
	int			last_return;
	int			pipe_count;
	int			hd_count;
	int			cancel_command;
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
void	free_tokens_til(t_token *start, t_token *end);
void	free_single_token(t_token *token);
void	free_tokens(t_minishell *shell);
t_token	*get_last_token(t_token *list);
int		ms_strchr(const char *src, int c);

// env
void	init_env(t_minishell *shell, char **env);
void	print_env(t_minishell *shell);
char	*ms_getenv(char *key, t_minishell *shell);
int		fill_key(t_env	*new);
int		fill_data(t_env	*new, int eq);
int		ms_replace_env(char	*beans, t_minishell *shell);

// execute.c
void	execute(t_command *cmd, t_minishell *shell);
void	execute_two_electric_boogaloo(t_minishell *shell);
// char	**get_command_options(t_token	*token);
t_token	*get_command_options(t_token *token, t_command *cmd);
// char	*pipex_pathjoin(char const *path, char const *cmd);
// ^^^^doesn't need to be in here probably.

// Builtins.c
bool	check_builtin(t_command *cmd, t_minishell *shell, t_from process);
int		ms_cd(t_command	*cmd, t_minishell *shell);
int		ms_echo(t_command *cmd);
int		ms_pwd(t_command *cmd);
int		ms_env(t_minishell	*shell, t_command	*cmd);
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

// commands.c
int		make_commands(t_minishell *shell);
void	print_commands(t_minishell *shell); //remove later

//env_utils.c
char    **create_envp(t_env *env_head);
void    print_envp(char **envp);
#endif

// void rl_replace_line (const char *text, int clear_undo);
/**
 * minishell steps

init minishell:

1: get input
2: tokenize input
3: expand input
4: resolve heredoc
5: prep execution
6: execute
7: return?

 **/