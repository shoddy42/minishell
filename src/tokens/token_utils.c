/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:32:49 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:53:14 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief	Merges tokens into one, by appending all token->data from start to end.
 * 			This does cause a loss of FDs within tokens, so only use it for literals.
 * 
 * @param start The token to start appending from. Will get freed.
 * @param end 	The token to append up to.
 * @param shell The shell.
 * @returns		The passed token @b [end], after it's data has been appended. 
 * 
 * @bug			NOT YET TESTED
 */
t_token	*merge_tokens(t_token *start, t_token *end, t_minishell *shell)
{
	t_token	*token;
	char	*new_data;

	token = start;
	new_data = ft_strdup("");
	while (token && new_data)
	{
		new_data = ft_strexpand(new_data, token->data);
		if (token == end || !token->next)
			break ;
		token = token->next;
	}
	if (!new_data)
		return (token_error(start, "Token merge error. ["));
	free (end->data);
	end->data = new_data;
	remove_tokens(start, end, shell);
	return (end);
}

/**
 * @brief	Frees all tokens from @b [START] up to @b [END]. 
 * 			Relinks tokens after deletion if needed.
 * 
 * @param start The first token to free.
 * 	 	If there is a start->prev, it will be linked to end->prev.
 * @param end 	The token to end on. This token does not get freed.
 * @param shell The shell.
 */
void	remove_tokens(t_token *start, t_token *end, t_minishell *shell)
{
	t_token	*tmp;
	t_token	*replace_prev;

	replace_prev = NULL;
	if (start->prev)
	{
		start->prev->next = end;
		replace_prev = start->prev;
	}
	else
		shell->token_head = end;
	while (start != end)
	{
		tmp = start;
		start = start->next;
		if (tmp->data)
			free(tmp->data);
		free(tmp);
	}
	end->prev = replace_prev;
}

/**
 * @brief Frees a single token in the list, and relinks tokens when needed.
 * 
 * @param token The token to be freed.
 */
void	free_single_token(t_token *token)
{
	if (token->prev && token->next)
	{
		token->prev->next = token->next;
		token->next->prev = token->prev;
	}
	else if (!token->next && token->prev)
		token->prev->next = NULL;
	else if (!token->prev && token->next)
		token->next->prev = NULL;
	free(token->data);
	free(token);
}

/**
 * @brief	Frees all tokens and their data
 *  		starting from shell->token_head
 *			And sets shell->token_head to NULL
 * 
 * @param shell The shell.
 */
void	free_tokens(t_minishell *shell)
{
	t_token	*token;

	token = shell->token_head;
	while (token && token->next)
	{
		if (token->data)
			free(token->data);
		token = token->next;
		if (token->prev)
			free(token->prev);
	}
	if (token)
	{
		if (token->data)
			free(token->data);
		free(token);
	}
	shell->token_head = NULL;
}

t_token	*get_last_token(t_token *list)
{
	t_token	*tmp;

	tmp = list;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}
