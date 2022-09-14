/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:32:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/14 01:52:40 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// REMOVE THIS FUNCTION LATER
void print_tokens(t_minishell *shell)
{
	int i = 0;
	t_token *test;

	test = shell->tokens;
	printf("all tokens: ");
	while (test && test->next)
	{
		// printf("(%i)", test->type);
		printf("[%s]-", test->data);
		test = test->next;
	}
	if (test)
	{
		// printf("(%i)", test->type);
		printf("[%s]\n", test->data);
	}
}

void	free_tokens(t_minishell *shell)
{
	t_token *list;

	list = shell->tokens;
	while (list && list->next)
	{
		if (list->data)
			free(list->data);
		list = list->next;
	}
	if (list)
		free(list);
	shell->tokens = NULL;
}

t_token	*get_last_token(t_token *list)
{
	t_token *tmp;

	tmp = list;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}
