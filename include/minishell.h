/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/12 10:56:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>  //mby?
# include <readline/readline.h> //required
# include <readline/history.h> //required

// #include
# include <unistd.h> //prolly?
# include <stdlib.h> //prolly?
# include <string.h> //?
# include <signal.h> //required
# include <fcntl.h>
# include "libft/libft.h"

# include <sys/wait.h> // needed for WSL

# define DELIMITER " |<>;\t\'\"\n"

typedef enum e_tokentype
{
    COMMAND = 42,	//currently all "words" are commands. probably should be renamed "WORD"
    QUOTE = 45,	 	//for '
    DQUOTE = 46, 	//for ""
	DOLLAR = 47,	//for $, probably should be renamed to "VARIABLE"
	AND = 48,		//for &
	LEFT = 49,		//for <
	RIGHT = 50,		//for >
    PIPE = 51,		//for |
	HEREDOC = 53,	//for >>
	HEREDOC_FILE = 54,
	OUTFILE,
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

typedef struct s_command
{
	char			*command; // not required, can use options[0] instead if want?
	char			**options;	
	int				outfile;
	int				intfile;
	struct s_token	*used_token;
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
	int			token_size;
	t_env		*env;
	char		**envp;
	char		**path;
	char		*command;
	int			last_return;
	int			pipe_count;
	int			exit;
	int			test;
}	t_minishell;


// tokenize.c
void	set_token_type(t_minishell *shell, t_token *token, char *data);
int		new_token(t_minishell *shell, char *data, int len);
void	ft_tokenize(t_minishell *shell, char *command);

// token_utils.c
void 	print_tokens(t_minishell *shell);
void	free_tokens_til(t_token *start, t_token *end);
void	free_single_token(t_token *token);
void	free_tokens(t_minishell *shell);
t_token	*get_last_token(t_token *list);

// env
void	init_env(t_minishell *shell, char  **env);
void	print_env(t_minishell *shell);

// execute.c
void    execute(t_command *cmd, t_minishell *shell);

// Builtins.c
int    check_builtin(t_command    *cmd, t_token *token);
int    ms_cd(t_command	*cmd);

// heredoc.c
t_token	*heredoc(t_token    *token, t_minishell *shell);
void    heredoc_loop(t_token    *token, t_minishell *shell);
void    ms_heredoc(t_token  *token, t_minishell *shell);

#endif


// void rl_replace_line (const char *text, int clear_undo);
/**
 * minishell steps
  
1: get input                 
2: tokenize input            
3: check which are executable
4: expand
5: execute
6: return?

 **/