/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:31:46 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/20 12:21:16 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	set_token_type(t_minishell *shell, t_token *token)
// {
// 	if (token->data[0] ==  ' ')
// 		token->type = VOID;
// 	else if (token->data[0] ==  '|')
// 		token->type = PIPE;
// 	else if (token->data[0] ==  '$')
// 		token->type = DOLLAR;
// 	else if (token->data[0] ==  '&')
// 		token->type = AND;
// 	else if (token->data[0] ==  '<')
// 		token->type = LEFT;
// 	else if (token->data[0] ==  '>')
// 		token->type = RIGHT;
// 	else if (token->data[0] ==  ';')
// 		token->type = VOID;
// 	else if (token->data[0] ==  '\'')
// 		token->type = QUOTE;
// 	else if (token->data[0] ==  '\"')
// 		token->type = DQUOTE;
// 	else if (token->data[0] ==  '\t')
// 		token->type = VOID;
// 	else if (token->data[0] ==  '\n')
// 		token->type = VOID;
// 	else
// 		token->type = COMMAND;
// }

void	set_token_type(t_minishell *shell, t_token *token, char *data)
{
	if (data[0] ==  ' ')
		token->type = VOID;
	else if (data[0] ==  '|')
		token->type = PIPE;
	else if (data[0] ==  '$')
		token->type = DOLLAR;
	else if (data[0] ==  '&')
		token->type = AND;
	else if (data[0] ==  '<')
		token->type = LEFT;
	else if (data[0] ==  '>')
		token->type = RIGHT;
	else if (data[0] ==  ';')
		token->type = VOID;
	else if (data[0] ==  '\'')
		token->type = QUOTE;
	else if (data[0] ==  '\"')
		token->type = DQUOTE;
	else if (data[0] ==  '\t')
		token->type = VOID;
	else if (data[0] ==  '\n')
		token->type = VOID;
	else
		token->type = COMMAND;
}

// int	handle_quote_token(t_minishell *shell, t_token *token, char *data)
// {
// 	int i;

// 	i = 0;
// 	while (data[++i])
// 		if (data[i] == '\'')
// 			break;
// 	if (data[i] == '\'')
// 		return (i + 1);
// 	printf ("UNCLOSED QUOTE!!\n");
// 	return (1);
// }

int		new_token(t_minishell *shell, char *data, int len)
{
	int		i;
	t_token	*new;
	t_token *last;

	i = -1;
	new = ft_calloc(1, sizeof(t_token));
	set_token_type(shell, new, data);
	// if (new->type == QUOTE)
	// 	len = handle_quote_token(shell, new, data);
	new->data = ft_calloc(len + 1, sizeof(char));
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
	// printf ("len = [%i] ", len);
	return (len);
}

// int		handle_quote_early(char *command, char c, int start)
// {
// 	int i;

// 	i = 1;
// 	printf("found quote\n");
// 	while (command[i + start] && command[i + start] != c)
// 		i++;
// 	if (command[i + start] != c)
// 		printf("NO CLOSING QUOTE\n");
// 	else
// 		i++;
// 	return (i);
// }

//only issue with this so far is that for the final token, it'll allocate 1 bit toomany.
void	ft_tokenize(t_minishell *shell, char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		while (ft_charinstr(command[i], DELIMITER) == 0 && command[i])
			i++;
		// if (command[i] == '\'' || command[i] == '\"')
		// 	i += handle_quote_early(command, command[i], i);
		if (ft_charinstr(command[i], DELIMITER) == 1 && i > 0)
			i--;
		command += new_token(shell, command, i + 1);
		// new_token(shell, command, i + 1);
		// command += i + 1;
		i = 0;
	}
}