/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:31:46 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:36:31 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_token_type(t_token *token, char *data)
{
	if (!data || !data[0])
		token->type = ERROR;
	if (ft_isspace(data[0]))
		token->type = VOID;
	else if (data[0] == '|')
		token->type = PIPE;
	else if (data[0] == '$')
		token->type = VARIABLE;
	else if (data[0] == '<')
		token->type = LEFT;
	else if (data[0] == '>')
		token->type = RIGHT;
	else if (data[0] == '\'')
		token->type = QUOTE;
	else if (data[0] == '\"')
		token->type = DQUOTE;
	else
		token->type = COMMAND;
}

void	link_token(t_minishell *shell, t_token *new)
{
	t_token	*last;

	last = get_last_token(shell->token_head);
	if (last != NULL)
	{
		last->next = new;
		new->prev = last;
	}
	else
	{
		new->next = NULL;
		shell->token_head = new;
	}	
}

t_token	*new_token(t_minishell *shell, char *data, int len, bool link)
{
	int		i;
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	set_token_type(new, data);
	new->data = ft_calloc(len + 1, sizeof(char));
	new->fd = -1;
	i = -1;
	while (++i < len)
		new->data[i] = data[i];
	if (link == true)
		link_token(shell, new);
	else
	{
		new->next = NULL;
		new->prev = NULL;
	}
	return (new);
}

void	tokenize(t_minishell *shell, char *line)
{
	int	i;
	int	len;

	i = 0;
	while (line[i])
	{
		len = 1;
		if (ft_charinstr(line[i], DELIM) == false && line[i + len])
			while (line[i + len] && ft_charinstr(line[i + len], DELIM) == false)
				len++;
		new_token(shell, line + i, len, true);
		i += len;
	}
}
