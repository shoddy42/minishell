/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 01:07:42 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 01:27:14 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks infront of, and behind the pipe
 *  to check whether there is a syntax error.
 * 
 * @param token A token of which token->type == PIPE 
 * @param shell The shell.
 */
void	check_pipe(t_token *token, t_minishell *shell)
{
	t_token	*forward;
	t_token	*behind;

	forward = token->next;
	behind = token->prev;
	if (!forward || !behind)
	{
		printf ("Syntax error [|]\n");
		shell->cancel_all_commands = true;
		return ;
	}
	while (forward && forward->next && forward->type != COMMAND
		&& forward->type != PIPE)
		forward = forward->next;
	while (behind && behind->next && behind->type != COMMAND
		&& forward->type != PIPE)
		behind = behind->next;
	if (forward->type != COMMAND || behind->type != COMMAND)
	{
		printf ("Syntax error [|]\n");
		shell->cancel_all_commands = true;
	}
	shell->cancel_command = false;
}

int	count_pipes(t_minishell *shell)
{
	t_token	*token;

	token = shell->token_head;
	shell->pipe_count = 0;
	while (token)
	{
		if (token->type == PIPE)
			shell->pipe_count++;
		if (!token->next)
			break ;
		token = token->next;
	}
	return (0);
}

/**
 * @brief Loops through all tokens after parsing. 
 * 		  Will append neighboring tokens of type COMMAND to eachother.
 * 
 * @param shell The shell.
 */
void	parse_append(t_minishell *shell)
{
	t_token	*tmp;

	tmp = shell->token_head;
	while (tmp && tmp->next)
	{
		if (tmp->type == COMMAND && tmp->next->type == COMMAND
			&& tmp->data && tmp->next->data)
		{
			tmp->data = ft_strexpand(tmp->data, tmp->next->data);
			free_single_token(tmp->next);
		}
		else
			tmp = tmp->next;
	}
}

/**
 * @brief	Reads through all tokens and their types.
 * 			 Calls all relevant handle functions in order of:
 * 			Currently supported token->types:
 *  [LEFT] <, [RIGHT] >, ([QUOTE] ', [DQUOTE] "), [VARIABLE] $, [PIPE] |,
 * 			Quote and DQuote are handled in the same step.
 * 
 * @param shell The shell.
 */
void	parse_token(t_minishell *shell)
{
	t_token	*token;

	token = shell->token_head;
	while (token && shell->cancel_all_commands == false)
	{
		if (token->type == LEFT)
			token = handle_left(token, shell);
		else if (token->type == RIGHT && shell->cancel_command == false)
			token = handle_right(token, shell);
		else if (token->type == QUOTE || token->type == DQUOTE)
			token = handle_quote(token, token->type, shell);
		else if (token->type == VARIABLE)
			expand_dong(token, shell);
		else if (token->type == PIPE)
			check_pipe(token, shell);
		if (token->type == ERROR)
			shell->cancel_command = true;
		if (!token || !token->next)
			break ;
		token = token->next;
	}
	parse_append(shell);
}
