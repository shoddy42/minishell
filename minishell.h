/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/09 17:13:24 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h> //required
# include <readline/history.h> //required
# include <stdio.h>  //mby?
# include <unistd.h> //prolly?
# include <stdlib.h> //prolly?
# include <string.h> //?
# include <signal.h> //required
# include "libft/libft.h"

# define DELIMITER "|$&<>;"

typedef enum e_tokentype
{
    COMMAND = 42,
    OPTION,
    ENV,
    QUOTE,
    DQUOTE,
    PIPE
}   t_tokentype;

typedef struct s_token
{
    char		*data;
	int			exe;
    t_tokentype	type;
}   t_token;

typedef struct s_command
{
	char *command;
	char **options;
	char *index;
}   t_command;

typedef struct s_shell_data
{
	t_command	*tokens;
	char		*testing;
	int			last_return;
	int			test;
}	t_minishell;


#endif

/**
 * minishell steps
  
1: get input                 
2: tokenize input            
3: check which are executable
4: expand
5: execute
6: return?

 **/