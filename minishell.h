/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/12 22:59:44 by wkonings      ########   odam.nl         */
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
# include "libft/libft.h"

# define DELIMITER " |$&<>;\'\"\t\n"

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
	char *command;
	char **options;
	char *index;
}   t_command;

typedef struct s_shell_data
{
	t_token		*tokens;
	char		*testing;
	int			last_return;
	int			test;
}	t_minishell;

typedef struct s_env
{
	char 	*key;
	char 	*beans;
	struct	s_env *next;
} t_env;


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