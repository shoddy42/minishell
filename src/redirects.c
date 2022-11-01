/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:05:15 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/01 11:35:31 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//later: will probably be fixed by the errors. but prevent creation of files like < > | /
t_token	*handle_left(t_token *token, t_minishell *shell)
{
	t_token *tmp;

	tmp = NULL;
	// printf("\nentered handle_left on token [%s]\n", token->data);
	if (token->next)
		tmp = token->next;
	if (!tmp)
	{
		ms_error("Bad redirect.", 0, FALSE, shell);
		return (token);
	}
	if (tmp->type == LEFT && tmp->next)
		tmp = heredoc(tmp, shell);
	else if (tmp->type == COMMAND)
	{
		// handling relinking the linked list. might turn this into a function of itself. OR add it to free_tokens_til.
		if (token->prev)
			tmp->prev = token->prev;
		else
			shell->tokens = tmp;
		free_tokens_til(token, tmp);
		tmp->fd = open(tmp->data, O_RDONLY);
		if (tmp->fd == -1)
		{
			ms_error("Failed to open file.", 0, FALSE, shell);
			return (token);
		}
		tmp->type = INFILE;
	}
	return (tmp);
}

//later: split function so it's norme
t_token	*handle_right(t_token *token, t_minishell *shell)
{
	t_token 	*tmp;
	int			append;

	append = 0;
	tmp = NULL;
	// printf("\nentered handle_right on token [%s]\n", token->data);
	if (token->next)
		tmp = token->next;
	if (!tmp)
	{
		ms_error("ERROR HANDLE_RIGHT, NO TOKEN", -5, FALSE, shell);
		return (token);
	}
	if (tmp->type == RIGHT)
	{
		append = 1;
		if (tmp->next)
			tmp = tmp->next;
	}
	while (tmp->type == VOID && tmp->next)
		tmp = tmp->next;
	if (tmp->type != COMMAND)
	{
		ms_error("REDIRECT FAILURE, NO FILENAME GIVEN", -6, FALSE, shell);
		return (token);
	}
	if (append == 1)
		tmp->fd = open(tmp->data, O_RDWR | O_APPEND | O_CREAT, 0644);
	else
		tmp->fd = open(tmp->data, O_RDWR | O_TRUNC | O_CREAT, 0644);
	tmp->type = OUTFILE;
	if (!token->prev) //ugly fix, but the problem is if token happens to be the HEAD (shell->tokens), we will segf.
		shell->tokens = tmp;
	free_tokens_til(token, tmp);
	return (tmp);
}