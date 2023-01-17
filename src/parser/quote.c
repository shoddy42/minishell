/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quote.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:58:28 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:53:33 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/**
 * @brief Writes error in case of no closing quote.
 */
void	quote_error(t_token *tmp, t_token *start,
	t_tokentype type, t_minishell *shell)
{
	if (tmp->type != type)
		printf ("WARNING: UNCLOSED QUOTE\n");
	remove_tokens(start->next, tmp, shell);
	free_single_token(tmp);
	start->next = NULL;
}

/**
 * @brief Goes through all tokens past @b [start],
 * 		  until a matching token of type @b [int] is found.
 * 		  Merges all found tokens into @b [start].
 * 
 * @param start The quote token.
 * @param type	DQUOTE or QUOTE.
 * @param shell The shell.
 * @return @b [start] after all tokens are merged.
 */
t_token	*handle_quote(t_token *start, t_tokentype type, t_minishell *shell)
{
	t_token	*tmp;
	char	*str;

	tmp = start;
	if (!tmp || !tmp->next)
		return (token_error(tmp, NULL));
	str = ft_calloc(1, 1);
	start->type = COMMAND;
	free(start->data);
	start->data = NULL;
	while (tmp && tmp->type != type)
	{
		if (type == DQUOTE && tmp->type == VARIABLE)
			expand_dong(tmp, shell);
		str = ft_strexpand(str, tmp->data);
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	if (tmp->type == type && tmp->next)
		remove_tokens(start->next, tmp->next, shell);
	else
		quote_error(tmp, start, type, shell);
	start->data = str;
	return (start);
}
