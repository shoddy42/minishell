/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quote.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:58:28 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/31 13:32:21 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//todo: make """" not become a space
//todo: figure out why [echo "cat traveller | cat] prints twice. i think it has to do with the quote.
//todo: "echo 192590189032" doesn't command not found.
t_token	*handle_quote(t_token *token, int type, t_minishell *shell)
{
	t_token	*tmp;
	char	*str;

	tmp = token;
	if (!tmp || !tmp->next)
	{
		printf("ERROR: OUT OF TOKENS\n");
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
			printf ("WARNING: UNCLOSED %s\n", print_token_type(type));
		free_tokens_til(token->next, tmp);
		free_single_token(tmp);
		token->next = NULL;
	}
	return (token);
}
