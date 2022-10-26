/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:59:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/26 13:34:02 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_dong(t_token *token, t_minishell *shell)
{
	char *tmp;

	if (token->data[1] && token->data[1] == '?')
	{
		free(token->data);
		token->type = COMMAND;
		token->data = ft_itoa(shell->last_return);
		return ;
	}
	tmp = ms_getenv(token->data + 1, shell);
	free(token->data);
	token->type = COMMAND; //might need other types too?
	if (!tmp)
		token->data = ft_strdup("");
	else
		token->data = ft_strdup(tmp);
}