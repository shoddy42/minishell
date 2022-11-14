/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:59:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/14 11:16:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*word_adding(char *variable, t_token *token, t_minishell *shell)
{
	t_token	*new;
	t_token	*space;

	new = new_token(shell, variable, ft_strlen(variable), false);
	if (!new)
		ms_error ("Failed to allocate token for split word", -24, true, shell);
	free(variable);
	space = new_token(shell, " ", 1, false);
	if (!space)
		ms_error ("Failed to allocate token for split word", -24, true, shell);
	token->next = space;
	space->prev = token;
	space->next = new;
	new->prev = space;
	return (new);
}

t_token	*word_splitting(char *variable, t_token *token, t_minishell *shell)
{
	char	**words;
	t_token	*test;
	t_token	*tmp;
	int		i;

	words = ft_split(variable, ' ');
	if (!words)
		ms_error("Failed wordsplitting variable", -23, true, shell);
	i = 0;
	tmp = token;
	test = NULL;
	if (token->next)
		test = token->next;
	if (words[i])
		tmp->data = words[i];
	else
	{
		tmp->data = ft_strdup("");
		tmp->type = VOID;
	}
	while (words[++i])
		tmp = word_adding(words[i], tmp, shell);
	free (words);
	return (tmp);
}

void	expand_dong(t_token *token, t_minishell *shell)
{
	char	*variable;
	t_token	*tmp;
	t_token	*next;

	token->type = COMMAND;
	if (token->next && token->next->type == COMMAND)
		tmp = token->next;
	else
		return ;
	next = NULL;
	if (tmp->next)
		next = tmp->next;
	if (tmp->data && tmp->data[0] == '?')
	{
		printf ("number? = [%i]\n", shell->last_return);
		variable = ft_itoa(shell->last_return);
	}
	else
		variable = ft_strdup(ms_getenv(tmp->data, shell));
	free_single_token(tmp);
	free(token->data);
	if (ft_strcmp("", variable) == 0)
		token->data = variable;
	else
	{
		tmp = word_splitting(variable, token, shell);
		free(variable);
		if (next)
		{
			tmp->next = next;
			next->prev = tmp;
		}
	}
}
