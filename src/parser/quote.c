/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quote.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:58:28 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/18 18:47:31 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//redo so it doesnt return starter token but scrolled token.
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
	free(token->data);
	token->data = NULL;
	while (tmp && tmp->type != type)
	{
		if (type == DQUOTE && tmp->type == VARIABLE)
			expand_dong(tmp, shell);
		str = ft_strexpand(str, tmp->data);
		if (!tmp->next)
			break;
		tmp = tmp->next;
	}
	if (tmp->type == type && tmp->next)
		free_tokens_til(token->next, tmp->next, shell);
	else //incase there is no token past "
	{
		if (tmp->type != type)
			printf ("WARNING: UNCLOSED %s\n", print_token_type(type));
		free_tokens_til(token->next, tmp, shell);
		free_single_token(tmp);
		token->next = NULL;
	}
	token->data = str;
	return (token);
}

// t_token	*handle_quote(t_token *token, int type, t_minishell *shell)
// {
// 	t_token	*tmp;
// 	t_bool	append_start;
// 	t_bool	append_end;
// 	char	*str;

// 	tmp = token;
// 	printf ("quote tok[%s]\n", token->data);
// 	if (!tmp || !tmp->next)
// 	{
// 		printf("ERROR: OUT OF TOKENS\n");
// 		return (token);
// 	}
// 	str = ft_calloc(1, 1); // required for WSL at least otherwise we segmafault on ft_strexpand
// 	token->type = COMMAND;
// 	if (tmp->prev && tmp->prev->type == COMMAND)
// 		tmp = tmp->prev;
// 	while (tmp && tmp->next && tmp->type != type)
// 	{
// 		if (type == DQUOTE)
// 			if (tmp->next->type == VARIABLE)
// 				expand_dong(tmp->next, shell);
// 		if (tmp->next->type != type)
// 			str = ft_strexpand(str, tmp->next->data);
// 		tmp = tmp->next;
// 	}
// 	token->data = str;
// 	if (tmp->type == type && tmp->next)
// 	{
// 		tmp = tmp->next;
// 		free_tokens_til(token->next, tmp);
// 		token->next = tmp;
// 	}
// 	else
// 	{
// 		if (tmp->type != type)
// 			printf ("WARNING: UNCLOSED %s\n", print_token_type(type));
// 		free_tokens_til(token->next, tmp);
// 		free_single_token(tmp);
// 		token->next = NULL;
// 	}
// 	return (token);
// }
