/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 05:59:42 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:55:51 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief 
 * 
 * @param token The token to set to type ERROR, and print the data of.
 * @param msg	The message to be printed on the STDERR. If there is no
 * 				message, it will perror the token->data instead.
 * @param print_token When @b [true]: prints the data of @b [token].
 * 					  When @b [false]: does nothing.
 * @return The passed @b [token] after being set to error.
 */
t_token	*token_error(t_token *token, char *msg)
{
	token->type = ERROR;
	if (msg)
	{
		write(2, msg, ft_strlen(msg));
		if (token->data)
			write(2, token->data, ft_strlen(token->data));
		write(2, "]", 1);
		write(2, "\n", 1);
	}
	else
		perror(token->data);
	return (token);
}

/**
 * @brief Simple perror with exit.
 * 
 * @param msg The message to perror.
 * @param error_code The code to exit with.
 * @param quit Whether exit should be called.
 */
void	ms_error(char *msg, int error_code, bool quit)
{
	if (error_code != 0)
	{
		errno = error_code;
		perror(msg);
	}
	if (quit == true)
		exit(error_code);
}
