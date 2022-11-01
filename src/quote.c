/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quote.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:58:28 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/01 12:51:38 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//todo: make """" not become a space
//todo: figure out why [echo "cat traveller | cat] prints twice. i think it has to do with the quote.
//todo: "echo 192590189032" doesn't command not found.

t_token	*handle_quote(t_token *token, int type, t_minishell *shell)
{
	t_token	*tmp;
	t_bool	append_start;
	t_bool	append_end;
	char	*str;

	append_end = FALSE;
	append_start = FALSE;
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
	if (tmp->prev && tmp->prev->type == COMMAND)
		append_start = TRUE;
	while (tmp && tmp->type != type)
	{
		if (type == DQUOTE)
			if (tmp->type == VARIABLE)
				expand_dong(tmp->next, shell);
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
