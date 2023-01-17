/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 17:24:48 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//LATER: check if we have any headers we dont need.

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../include/prompt.h"
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
# include "../libft/include/libft.h"
# include <stdbool.h>
# include <limits.h>
# include <dirent.h>
# include <termios.h>

// ERROR DEFINES
# define SYN_NODELIM "Syntax error near '<<'; No valid DELIM for heredoc. ["
# define SYN_LUNEX "Syntax error near '<'; Unexpected token: ["
# define REDIR_R_DENIED "Redirect '>' failed; Permission denied for file: ["
# define REDIR_R_NOFILE "Redirect '>' failed; No such file or directory: ["
# define SHELL_ALLOC_FAILURE "FAILED TO ALLOCATE SHELL, YOU HAVE 0 MEMORY LMAO"

# define DELIM " |$<>=;\t\'\"\n"

typedef enum e_pipe
{
	READ = 0,
	WRITE = 1,
	NEEDS_PIPE = -42
}	t_pipe;

typedef enum e_tokentype
{
	COMMAND,
	QUOTE,
	DQUOTE,
	VARIABLE,
	AND,
	LEFT,
	RIGHT,
	PIPE,
	HEREDOC,
	SEMICOLON,
	OUTFILE,
	INFILE,
	ERROR,
	VOID,
}	t_tokentype;

typedef struct s_token
{
	char			*data;
	int				fd;
	t_tokentype		type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

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
	char		*bin_dir;
	DIR			*bin;

	bool		hd_history;
	int			prompt_type;
	char		*prompt;
	pid_t		last_cmd;
	int			pipe_count;
	int			hd_count;
	bool		cancel_command;
	bool		cancel_all_commands;
	int			exit;
}	t_minishell;

// signal.c
void		child_sig(int signum);
void		sighandler(int signum);
void		heredoc_sig(int signum);

// init.c
t_minishell	*init_minishell(char **envp);
void		create_bin(t_minishell *shell);

// error.c 
void		ms_error(char *msg, int code, bool terminate);
t_token		*token_error(t_token *token, char *msg);

// cleanup.c
void		ms_cleanup(t_minishell *shell);
void		free_commands(t_minishell *shell);

// BUILTINS

int			close_builtin(t_command *cmd);
bool		is_builtin(t_command *cmd, t_minishell *shell);
int			ms_cd(t_command	*cmd, t_minishell *shell);
int			ms_echo(t_command *cmd);
int			ms_pwd(t_minishell *shell);
int			print_env(t_minishell	*shell, t_command	*cmd);
int			ms_export(t_command *cmd, t_minishell	*shell);
int			ms_unset(t_minishell *shell, t_command *cmd);

// prompt.c
bool		change_prompt(t_command *cmd, t_minishell *shell);

t_token		*new_token(t_minishell *shell, char *data, int len, bool link);
void		tokenize(t_minishell *shell, char *command);

// token_utils.c
void		remove_tokens(t_token *start, t_token *end, t_minishell *shell);
void		free_single_token(t_token *token);
void		free_tokens(t_minishell *shell);
t_token		*get_last_token(t_token *list);
int			ms_strchr(const char *src, int c);
t_token		*merge_tokens(t_token *start, t_token *end, t_minishell *shell);

// parser.c
void		parse_append(t_minishell *shell);
int			count_pipes(t_minishell *shell);
void		parse_token(t_minishell *shell);

// env
t_env		*env_exists(char	*beans, t_minishell *shell);
char		*ms_getenv(char *key, t_minishell *shell);
int			fill_env(t_env	*new);
int			new_env(char *data, t_minishell *shell);
bool		ms_replace_env(char *beans, t_minishell *shell);
int			replace_env(char *beans, t_env *env);
void		print_export(t_minishell	*shell);

// envp.c
void		create_envp(t_minishell *shell);

// commands.c
int			make_commands(t_minishell *shell);

// execute.c
void		execute_two_electric_boogaloo(t_minishell *shell);

// execute_utils.c
void		parent_close(t_command *cmd, t_minishell *shell);
char		*pipex_pathjoin(char const *path, char const *cmd);
bool		handle_path(char **args, char **envp);
void		local_command(char **args, char **envp);
void		set_status(t_minishell *shell, int cmd_count);

// heredoc.c
t_token		*heredoc(t_token *token, t_minishell *shell);
void		heredoc_loop(t_token *token, t_minishell *shell);
void		ms_heredoc(t_token *token, t_minishell *shell);
void		delete_heredocs(t_minishell *shell);

// heredoc_utils.c
char		*get_hd_name(t_minishell	*shell);
void		delete_heredocs(t_minishell *shell);
int			hd_var(int fd, char *line, t_minishell *shell);

// redirects.c
t_token		*handle_left(t_token *token, t_minishell *shell);
t_token		*handle_right(t_token *token, t_minishell *shell);

// quote.c
t_token		*handle_quote(t_token *token, t_tokentype type, t_minishell *shell);

// expand.c
void		expand_dong(t_token *token, t_minishell *shell);

//env_utils.c
void		create_envp(t_minishell *shell);

#endif