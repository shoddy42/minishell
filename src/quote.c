/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quote.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:58:28 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/26 11:24:40 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*handle_quote(t_token *token, int type, t_minishell *shell)
{
	t_token	*tmp;
	char	*str;

	tmp = token;
	if (!tmp->next)
		//printf("GIGA HUGE ERROR NOW\n\n\n\n\n");
	if (!tmp || !tmp->next)
	{
		//printf("ERROR: OUT OF TOKENS\n");
		return (token);
	}
	str = ft_calloc(1, 1); // required for WSL at least otherwise we segmafault on ft_strexpand
	token->type = COMMAND;
	while (tmp && tmp->next && tmp->type != type)
	{
		if (type == DQUOTE)
			if (tmp->next->type == VARIABLE)
				expand_dong(tmp->next, shell);
		if (tmp->next->type != type)
			str = ft_strexpand(str, tmp->next->data);
		tmp = tmp->next;
	}
	token->data = str;
	if (tmp->type == type && tmp->next)
	{
			tmp = tmp->next;
			free_tokens_til(token->next, tmp);
			token->next = tmp;
	}
	else
	{
		if (tmp->type != type)
			//printf ("WARNING: UNCLOSED type\n");
		free_tokens_til(token->next, tmp);
		free_single_token(tmp);
		token->next = NULL;
	}
	return (token);
}
