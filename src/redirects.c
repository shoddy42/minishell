/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:05:15 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/15 11:01:51 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//todo: fix segfault on 
//todo: WAY MORE ERROR HANDLING!!! <<<<<<<<<<< >>>>>>>>>>>> <><><>
t_token	*handle_left(t_token *token, t_minishell *shell)
{
	t_token	*tmp;

	// printf ("enter handle left on tok (%s)[%s]\n", print_token_type(token->type), token->data);
	tmp = NULL;
	if (token->next)
		tmp = token->next;
	//weird error?
	if (!tmp)
	{
		ms_error("Bad redirect.", 0, false, shell);
		token->type = ERROR;
		return (token);
	}
	if (tmp->type == LEFT && tmp->next)
		tmp = heredoc(tmp, shell);
	// printf ("here!\n");
	while (tmp && tmp->next && tmp->type == VOID)
		tmp = tmp->next;
	if (tmp->type == COMMAND)
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
			ms_error("Failed to open infile.", 0, false, shell);
			tmp->type = ERROR;
			return (tmp); //segfault here?
		}
		tmp->type = INFILE;
	}
	else
	{
		tmp->type = ERROR;
		printf ("syntax error near '<'\n");
	}
	return (tmp);
}

t_token	*handle_right(t_token *token, t_minishell *shell)
{
	t_token		*tmp;
	int			append;

	append = 0;
	tmp = NULL;
	// printf("\nentered handle_right on token [%s]\n", token->data);
	if (token->next)
		tmp = token->next;
	if (!tmp)
	{
		ms_error("ERROR HANDLE_RIGHT, NO TOKEN", -5, false, shell);
		token->type = ERROR;
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
		ms_error("REDIRECT FAILURE, NO FILENAME GIVEN", -6, false, shell);
		token->type = ERROR;
		return (token);
	}
	if (append == 1)
		tmp->fd = open(tmp->data, O_RDWR | O_APPEND | O_CREAT, 0644);
	else
		tmp->fd = open(tmp->data, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (tmp->fd < 0)
	{
		write (2, "minishell: ", 12);
		write (2, tmp->data, ft_strlen(tmp->data));
		write (2, ": Permission denied\n", 20);
		tmp->type = ERROR;
		return (tmp);
	}
	tmp->type = OUTFILE;
	if (!token->prev) //ugly fix, but the problem is if token happens to be the HEAD (shell->tokens), we will segf.
		shell->tokens = tmp;
	free_tokens_til(token, tmp);
	return (tmp);
}