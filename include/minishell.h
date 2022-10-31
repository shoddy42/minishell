/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/31 13:01:55 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>  //mby?
# include <readline/readline.h> //required
# include <readline/history.h> //required
# include <errno.h>
# include <unistd.h> //prolsly?
# include <stdlib.h> //prolly?
# include <string.h> //?
# include <signal.h> //required
# include <fcntl.h>
// # include <sys/wait.h>
# include <sys/types.h>
# include "../libft/libft.h"

// # include <sys/wait.h> // needed for WSL

# define DELIMITER " |<>\t\'\"\n"

typedef	enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef enum e_from
{
	MINISHELL,
	CHILD
}	t_from;

typedef	enum e_pipe
{
	READ = 0,
	WRITE = 1,
	NEEDS_PIPE = -42
}	t_pipe;

typedef enum e_tokentype
{
    COMMAND = 42,	//currently all "words" are commands. probably should be renamed "WORD"
    QUOTE = 45,	 	//for '
    DQUOTE = 46, 	//for ""
	VARIABLE = 47,	//for $
	AND = 48,		//for &
	LEFT = 49,		//for <
	RIGHT = 50,		//for >
    PIPE = 51,		//for |
	HEREDOC = 53,	//for >>
	HEREDOC_FILE = 54,
	OUTFILE = 55,
	INFILE = 56,
	ERROR = 57,
	EXIT_STATUS,
	VOID = 69,		//any whitespace.
}   t_tokentype;

typedef struct s_token
{
    char			*data;
	int				fd;
    t_tokentype		type;
	struct s_token	*next;
	struct s_token	*prev;
}   t_token;

// adding a total of exactly 5 ints and no other ints seems to segfault us for freeing something htat doesnt exist BUT we never free it. 
typedef struct s_command
{
	char				**command;	
	int					executable;
	int					outfile;
	int					infile;
	int					tunnel[2];
	pid_t				pid;
	struct	s_command	*next;
	struct	s_command	*prev;	
}   t_command;


typedef struct s_env
{
	char 	*key; // not yet in use, might need.
	char 	*beans; // not yet in use, might need.
	char	*data;
	struct	s_env *next;
} t_env;

typedef struct s_shell_data
{
	t_token		*tokens;
	t_command	*commands;
	t_env		*env;
	char		**envp;		// i dont think this ever REALLY gets used?
	char		**path;
	char		*command;

	int			last_return;
	int			pipe_count;
	int			cancel_command_line; //rename
	int			exit;
}	t_minishell;


// tokenize.c
void	set_token_type(t_minishell *shell, t_token *token, char *data);
int		new_token(t_minishell *shell, char *data, int len);
void	ft_tokenize(t_minishell *shell, char *command);

// token_utils.c
void 	print_tokens(t_minishell *shell);			//REMOVE LATER
void	print_tokens_backwards(t_minishell *shell); //REMOVE LATER
char 	*print_token_type(int type);				// REMOVE LATER
void	free_tokens_til(t_token *start, t_token *end);
void	free_single_token(t_token *token);
void	free_tokens(t_minishell *shell);
t_token	*get_last_token(t_token *list);
int		ms_strchr(const char *src, int c);


// env
void	init_env(t_minishell *shell, char  **env);
void	print_env(t_minishell *shell);
char	*ms_getenv(char *key, t_minishell *shell);
char	*fill_key(char	*beans);
char	*fill_data(char	*beans);

// execute.c
void    execute(t_command *cmd, t_minishell *shell);
void    execute_two_electric_boogaloo(t_minishell *shell);
// char	**get_command_options(t_token	*token);
t_token	*get_command_options(t_token *token, t_command *cmd);
char	*pipex_pathjoin(char const *path, char const *cmd); //doesn't need to be in here probably.

// Builtins.c
int	check_builtin(t_command *cmd, t_minishell *shell, t_from process);
int	ms_cd(t_command	*cmd);
int	ms_echo(t_command *cmd);
int	ms_pwd(t_command *cmd);
int	ms_env(t_minishell	*shell, t_command	*cmd);
int	ms_export(t_command *cmd, t_minishell	*shell);
int ms_unset(t_minishell *shell, t_command *cmd);

// heredoc.c
t_token	*heredoc(t_token    *token, t_minishell *shell);
void    heredoc_loop(t_token    *token, t_minishell *shell);
void    ms_heredoc(t_token  *token, t_minishell *shell);

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

// error.c 
void    ms_error(char *msg, int code, t_bool terminate, t_minishell *shell);

// commands.c
int		make_commands(t_minishell *shell);
void	print_commands(t_minishell *shell); //remove later

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