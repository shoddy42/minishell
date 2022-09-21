/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/21 20:05:03 by wkonings      ########   odam.nl         */
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
    COMMAND = 42,
    OPTION = 43,
    ENV = 44,
    QUOTE = 45,
    DQUOTE = 46,
	DOLLAR = 47,
	AND = 48,
	LEFT = 49,
	RIGHT = 50,
    PIPE = 51,
	VOID = 52
}   t_tokentype;

typedef struct s_token
{
    char			*data;
    t_tokentype		type;
	struct s_token	*next;
	struct s_token	*prev;
}   t_token;

typedef struct s_command
{
	char			*command; // not required, can use options[0] instead if want?
	char			**options;
	struct s_token	*used_token;
	// char *index;
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
	t_env		*env;
	char		**envp;
	char		**path;
	int			last_return;
	int			test;
}	t_minishell;


// tokenize.c
void	set_token_type(t_minishell *shell, t_token *token, char *data);
int		new_token(t_minishell *shell, char *data, int len);
void	ft_tokenize(t_minishell *shell, char *command);

// token_utils.c
void	free_tokens(t_minishell *shell);
t_token	*get_last_token(t_token *list);
void 	print_tokens(t_minishell *shell);
void	free_next_tokens(t_token *tokens);

// env
void	init_env(t_minishell *shell, char  **env);
void	print_env(t_minishell *shell);

// execute.c
void    execute(t_command *cmd, t_minishell *shell);

//PIPEX
// char	*pipex_pathjoin(char const *path, char const *cmd);
// void	pipex_error(char *error_message, int mode);
// int		pipex_heredoc(char *av, t_ppx *pipex);
// void	pipex_open(int ac, char **av, t_ppx *pipex);
// int		split_path(char *env[], t_ppx *pipex);
// int		get_fd_in(int current_fd, t_ppx *pipex, int mode);

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



//-L /Users/$(USER)/.brew/opt/readline/lib -I /Users/$(USER)/.brew/opt/readline/include