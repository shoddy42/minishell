/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:05:15 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/18 20:36:06 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//todo: maybe? "ls > FAKE_FOLDER/test" gives "permission denied", rather than no such file or dir?
//todo: maybe Failed to open should be on perror instead of a write but eh.
t_token	*handle_left(t_token *start, t_minishell *shell)
{
	t_token	*token;

	if (start->next)
		token = start->next;
	else
		token = start;
	if (token->type == LEFT && token != start)
		return (heredoc(token, shell));
	// skip void + handle variable + quotes if needed can turn this block into a function for redir.
	while (token && token->next && token->type == VOID)
		token = token->next;
	if (token->type == VARIABLE)
		expand_dong(token, shell);
	if (token->type == QUOTE || token->type == DQUOTE)
	{
		token = handle_quote(token, token->type, shell);
		parse_append(shell);
	}
	// actually open.
	if (token->type == COMMAND)
	{
		free_tokens_til(start, token, shell);
		token->fd = open(token->data, O_RDONLY);
		token->type = INFILE;
		if (token->fd == -1)
			return(token_error(token, "Failed to open infile: [", true));
		close (token->fd);
	}
	else
	{
		shell->cancel_all_commands = true;
		return (token_error(token, "Syntax error near '<'; Unexpected token: [", true));
	}
	return (token);
}


//todo: all of this tbh
t_token	*handle_right(t_token *start, t_minishell *shell)
{
	t_token	*token;
	bool	append;

	append = false;
	if (start->next)
		token = start->next;
	else
		token = start;
	if (token->type == RIGHT && token != start)
	{
		append = true;
		if (token->next)
			token = token->next;
	}
	// skip void, handle expansion and quotes when needed.
	while (token && token->next && token->type == VOID)
		token = token->next;
	if (token->type == VARIABLE)
		expand_dong(token, shell);
	if (token->type == QUOTE || token->type == DQUOTE)
	{
		token = handle_quote(token, token->type, shell);
		parse_append(shell);
	}
	// error
	if (token->type != COMMAND)
	{
		shell->cancel_all_commands = true;
		return (token_error(token, "Syntax error near '>'; Unexpected token: [", true));
	}
	// actual open + error
	if (append)
		token->fd = open(token->data, O_RDWR | O_APPEND | O_CREAT, 0644);
	else
		token->fd = open(token->data, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (token->fd < 0)
		return (token_error(token, "Redirect '>' failed; Permission denied for file: [", true));
	// printf ("handling right\n");
	token->type = OUTFILE;
	close (token->fd);
	free_tokens_til(start, token, shell);
	return (token);
}
