/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:17:11 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/08 22:50:43 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
#include <signal.h>

typedef enum e_token
{
    COMMAND = 42,
    OPTION,
    ENV,
    QUOTE,
    DQUOTE,
    PIPE
}   t_token_names;

typedef struct s_shell_data
{
    int test;

    
} t_data;


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