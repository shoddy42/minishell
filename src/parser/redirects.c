/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:05:15 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:54:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*open_left(t_token *start, t_token *token, t_minishell *shell)
{
	if (token->type == COMMAND)
	{
		remove_tokens(start, token, shell);
		token->fd = open(token->data, O_RDONLY);
		token->type = INFILE;
		if (token->fd == -1)
			return (token_error(token, "Failed to open infile: ["));
		close (token->fd);
	}
	else
	{
		shell->cancel_all_commands = true;
		return (token_error(token, SYN_LUNEX));
	}
	return (token);
}

t_token	*handle_left(t_token *start, t_minishell *shell)
{
	t_token	*token;

	if (start->next)
		token = start->next;
	else
		token = start;
	if (token->type == LEFT && token != start)
		return (heredoc(token, shell));
	while (token && token->next && token->type == VOID)
		token = token->next;
	if (token->type == VARIABLE)
		expand_dong(token, shell);
	if (token->type == QUOTE || token->type == DQUOTE)
	{
		token = handle_quote(token, token->type, shell);
		parse_append(shell);
	}
	return (open_left(start, token, shell));
}

t_token	*right(t_token *start, t_token *token, t_minishell *shell, bool append)
{
	if (token->type == VARIABLE)
		expand_dong(token, shell);
	if (token->type == QUOTE || token->type == DQUOTE)
	{
		token = handle_quote(token, token->type, shell);
		parse_append(shell);
	}
	if (token->type != COMMAND)
	{
		shell->cancel_all_commands = true;
		return (token_error(token, "Syntax error near '>';\
			Unexpected token: ["));
	}
	token->fd = open(token->data, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (token->fd < 0 && errno == 13)
		return (token_error(token, REDIR_R_DENIED));
	else if (token->fd < 0)
		return (token_error(token, REDIR_R_NOFILE));
	close(token->fd);
	if (append && token->fd > 0)
		token->fd = O_APPEND;
	else if (token->fd > 0)
		token->fd = O_TRUNC;
	token->type = OUTFILE;
	remove_tokens(start, token, shell);
	return (token);
}

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
	while (token && token->next && token->type == VOID)
		token = token->next;
	return (right(start, token, shell, append));
}
