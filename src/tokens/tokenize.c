/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:31:46 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/21 17:47:18 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//todo: move this to libft lmao
int		ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	set_token_type(t_minishell *shell, t_token *token, char *data)
{
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
	// else if (data[0] == ';') //supported if uncomment all SEMICOLON occurances. not fully operational
	// 	token->type = SEMICOLON;
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
	set_token_type(shell, new, data);
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

//only issue with this so far is that for the final token, it'll allocate 1 bit too many.
//REWRITE... INCREMENTING  COMMAND CAUSES BIG ISSUES
// void	ft_tokenize(t_minishell *shell, char *command)
// {
// 	int	i;
// 	int	skip;

// 	i = 0;
// 	skip = 0;
// 	shell->pipe_count = 0;
// 	while (command[i])
// 	{
// 		printf ("pre: command[%i] = [%c]\n", i, command[i]);
// 		while (ft_charinstr(command[i], DELIMITER) == 0 && command[i] != '\0')
// 			i++;
// 		if (ft_charinstr(command[i], DELIMITER) == 1 && i > 0)
// 			i--;
// 		// command += new_token(shell, command, i + 1); //could swap back to this 1 line saver B]
// 		new_token(shell, command, i + 1, true);
// 		command += i + 1 + skip;
// 		printf ("post: command[%i] = [%c]\n", i, command[i]);
// 		skip = 0;
// 		i = 0;
// 	}
// }


// "echo hello world | ls -la | cat"
// [echo][ ][hello][ ][world][ ][|][ ][ls][ ][-la][ ][|][ ][cat]
void	ft_tokenize(t_minishell *shell, char *command)
{
	int	i;
	int	len;

	i = 0;
	shell->pipe_count = 0;
	while (command[i])
	{
		len = 1;
		if (ft_charinstr(command[i], DELIMITER) == false && command[i + 1])
			while (command[i + len] && ft_charinstr(command[i + len], DELIMITER) == false)
				len++;
		new_token(shell, command + i, len, true);
		i += len;
	}
}

