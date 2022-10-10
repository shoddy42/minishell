/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:32:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/22 16:12:51 by wkonings      ########   odam.nl         */
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
		if (test->type && test->type != VOID)
		{
			printf("(%i)", test->type);
			printf("[%s]-", test->data);
		}
		test = test->next;
	}
	if (test != NULL)
	{
		if (test->type && test->type != VOID)
		{
			// printf("final token ");
			printf("(%i)", test->type);
			printf("[%s]\n", test->data);
		}
	}
	printf("\n");
}

// free tokens starting from START, up to END, does NOT free END
void	free_tokens_til(t_token *start, t_token *end)
{
	t_token *tmp;

	while (start != end)
	{
		tmp = start;
		start = start->next;
		if (tmp->data)
			free(tmp->data);
		free(tmp);
	}
}

void	free_single_token(t_token *token)
{
	free(token->data);
	token->next = NULL;
	token->prev = NULL;
	free(token);
}

void	free_tokens(t_minishell *shell)
{
	t_token *list;

	list = shell->tokens;
	while (list && list->next)
	{
		if (list->data)
			free(list->data);
		// if (list->prev)
		// 	list->prev = NULL;
		list = list->next;
	}
	if (list)
		free(list);
	// shell->tokens->next = NULL;
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
