/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 05:59:42 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/29 21:12:16 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief 
 * 
 * @param token The token to set to type ERROR, and print the data of.
 * @param msg	The message to be printed on the STDERR.
 * @param print_token When @b [true]: prints the data of @b [token].
 * 					  When @b [false]: does nothing.
 * @return The passed @b [token] after being set to error.
 * //todo: remove the bool print_token,
 *  its useless. just swap to if (token->data) instead.
 */
t_token	*token_error(t_token *token, char *msg, bool print_token)
{
	token->type = ERROR;
	if (msg)
	{
		write(2, msg, ft_strlen(msg));
		if (print_token == true)
		{
			write(2, token->data, ft_strlen(token->data));
			write(2, "]", 1);
		}
		write(2, "\n", 1);
	}
	else
		perror(token->data);
	return (token);
}

//later: evaluate whether this is even needed at all.
void	ms_error(char *msg, int error_code, bool quit, t_minishell *shell)
{
	if (error_code != 0)
	{
		errno = error_code;
		perror(msg);
	}
	if (quit == true)
		exit(error_code);
}
