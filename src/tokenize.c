/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:31:46 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/28 10:46:28 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_isspace(char c)
{
	return(c == ' ' || c == '\t' || c == '\n');
}

void	set_token_type(t_minishell *shell, t_token *token, char *data)
{
	if (ft_isspace(data[0]))
		token->type = VOID;
	// if (data[0] == ' ')
	// 	token->type = VOID;
	else if (data[0] == '|')
	{
		shell->pipe_count++;
		token->type = PIPE;
	}
	else if (data[0] == '$')
		token->type = VARIABLE;
	// else if (data[0] == '&') //currently not needed
	// 	token->type = AND;
	else if (data[0] == '<')
		token->type = LEFT;
	// else if (strncmp(data, ">>", 2) == 0)
	else if (data[0] == '>')
		token->type = RIGHT;
	// else if (data[0] == ';') //probably not needed
		// token->type = VOID;
	else if (data[0] == '\'')
		token->type = QUOTE;
	else if (data[0] == '\"')
		token->type = DQUOTE;
	// else if (data[0] == '\t')
	// 	token->type = VOID;
	// else if (data[0] == '\n')
	// 	token->type = VOID;
	else
		token->type = COMMAND;
}

int	new_token(t_minishell *shell, char *data, int len)
{
	int		i;
	t_token	*new;
	t_token	*last;

	i = -1;
	new = ft_calloc(1, sizeof(t_token));
	set_token_type(shell, new, data);
	new->data = ft_calloc(len + 1, sizeof(char));
	new->fd = -1;
	while (++i < len)
		new->data[i] = data[i];
	// split this part off either back into ft_tokenize, or into a new func link_token
	last = get_last_token(shell->tokens);
	if (last != NULL)
	{
		last->next = new;
		new->prev = last;
	}
	else
	{
		new->next = NULL;
		shell->tokens = new;
	}
	return (len);
}

//only issue with this so far is that for the final token, it'll allocate 1 bit toomany.
//todo: make pipe_count not go up with pipes are are in quotes.
void	ft_tokenize(t_minishell *shell, char *command)
{
	int	i;
	int	skip;

	i = 0;
	skip = 0;
	shell->pipe_count = 0;
	while (command[i])
	{
		while (ft_charinstr(command[i], DELIMITER) == 0 && command[i])
			i++;
		if (ft_charinstr(command[i], DELIMITER) == 1 && i > 0)
			i--;
		// command += new_token(shell, command, i + 1);
		new_token(shell, command, i + 1);
		command += i + 1 + skip;
		skip = 0;
		i = 0;
	}
	//printf("TOTAL PIPES: %i\n", shell->pipe_count);
}
