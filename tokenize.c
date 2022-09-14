/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:31:46 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/14 01:52:06 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_token_type(t_minishell *shell, t_token *token)
{
	if (token->data[0] ==  ' ')
		token->type = VOID;
	else if (token->data[0] ==  '|')
		token->type = PIPE;
	else if (token->data[0] ==  '$')
		token->type = DOLLAR;
	else if (token->data[0] ==  '&')
		token->type = AND;
	else if (token->data[0] ==  '<')
		token->type = LEFT;
	else if (token->data[0] ==  '>')
		token->type = RIGHT;
	else if (token->data[0] ==  ';')
		token->type = VOID;
	else if (token->data[0] ==  '\'')
		token->type = VOID;
	else if (token->data[0] ==  '\"')
		token->type = VOID;
	else if (token->data[0] ==  '\t')
		token->type = VOID;
	else if (token->data[0] ==  '\n')
		token->type = VOID;
	else
		token->type = COMMAND;
}

void	new_token(t_minishell *shell, char *data, int len)
{
	int		i;
	t_token	*new;
	t_token *last;

	i = -1;
	new = ft_calloc(1, sizeof(t_token));
	new->data = ft_calloc(len + 1, sizeof(char));
	while (++i < len)
		new->data[i] = data[i];
	last = get_last_token(shell->tokens);
	if (last != NULL)
	{
		last->next = new;
		new->prev = last;
	}
	else
		shell->tokens = new;
	set_token_type(shell, new);
}

//only issue with this so far is that for the final token, it'll allocate 1 bit too many.
void	ft_tokenize(t_minishell *shell, char *command)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (command[i])
	{
		while (ft_charinstr(DELIMITER, command[i]) == 0 && command[i])
			i++;
		if (ft_charinstr(DELIMITER, command[i]) == 1 && i > 0)
			i--;
		if (!(command[0] == ' ' && i == 0))
			new_token(shell, command, i + 1);
		command += i + 1;
		i = 0;
	}
}