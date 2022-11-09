/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:59:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/09 05:12:15 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*word_adding(char *variable, t_token *token, t_minishell *shell)
{
	t_token *new;
	t_token *space;

	new = new_token(shell, variable, ft_strlen(variable), false);
	if (!new)
		ms_error ("Failed to allocate token for split word", -24, true, shell);
	space = new_token(shell, " ", 1, false);
	if (!space)
		ms_error ("Failed to allocate token for split word", -24, true, shell);
	token->next = space;

	space->prev = token;
	space->next = new;
	
	new->prev = space;
	return (new);
}

//todo: make sure words gets freed;
t_token	*word_splitting(char *variable, t_token *token, t_minishell *shell)
{
	char	**words;
	t_token *test;
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
		// tmp->data = ft_strdup(words[i]); //consider using tmp->data = words[i] and then only freeing words, instead of reallocing like this?
	while (words[++i])
		tmp = word_adding(words[i], tmp, shell);
	free (words);
	return (tmp);
}

// currently ms_getenv
void	expand_dong(t_token *token, t_minishell *shell)
{
	char	*variable;
	t_token *tmp;
	t_token *next;

	token->type = COMMAND; //not true by default.
	if (token->next)
		tmp = token->next;
	else
		return ;
	next = NULL;
	if (tmp->next)
		next = tmp->next;
	if (tmp->type == COMMAND)
	{
		if (tmp->data && tmp->data[0] == '?')
			variable = ft_itoa(shell->last_return);
		else
			variable = ms_getenv(tmp->data, shell);
		free_single_token(tmp);
	}
	else
		variable = "";
	free(token->data);
	tmp = word_splitting(variable, token, shell);
	if (next)
	{
		tmp->next = next;
		next->prev = tmp;
	}
}
