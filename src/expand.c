/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:59:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/17 14:59:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_dong(t_token *token)
{
	char *tmp;

	tmp = getenv(token->data + 1);
	free(token->data);
	token->type = COMMAND; //might need other types too?
	if (!tmp)
		token->data = ft_strdup("");
	else
		token->data = ft_strdup(tmp);
}