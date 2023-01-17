/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:59:49 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:19:55 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*word_adding(char *variable, t_token *token, t_minishell *shell)
{
	t_token	*new;
	t_token	*space;

	new = new_token(shell, variable, ft_strlen(variable), false);
	if (!new)
		ms_error ("Failed to allocate token for split word", -24, true);
	free(variable);
	space = new_token(shell, " ", 1, false);
	if (!space)
		ms_error ("Failed to allocate token for split word", -24, true);
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
		ms_error("Failed wordsplitting variable", -23, true);
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

void	expand_dong(t_token *start, t_minishell *shell)
{
	char		*variable;
	t_token		*token;
	extern int	g_status;

	start->type = COMMAND;
	if (start->next && start->next->type == COMMAND)
		token = start->next;
	else
		return ;
	if (token->data && token->data[0] == '?')
		variable = ft_itoa(g_status);
	else
		variable = ft_strdup(ms_getenv(token->data, shell));
	free_single_token(token);
	free(start->data);
	if (ft_strcmp("", variable) == 0)
		start->data = variable;
	else
	{
		token = word_splitting(variable, start, shell);
		free(variable);
	}
}
