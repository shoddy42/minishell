/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:32:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/21 18:56:36 by wkonings      ########   odam.nl         */
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
		// if (test->type && test->type != VOID)
		// {
			// printf("(%i)", test->type);
			printf("[%s]-", test->data);
		// }
		test = test->next;
	}
	if (test != NULL)
	{
		// if (test->type && test->type != VOID)
		// {
			// printf("final token ");
			// printf("(%i)", test->type);
			printf("[%s]\n", test->data);
		// }
	}
	printf("\n");
}

void	free_single_token(t_token *token)
{
	free(token->data);
	token->next = NULL;
	token->prev = NULL;
	free(token);
}

void	free_next_tokens(t_token *tokens)
{
	t_token	*list;

	list = tokens;
	while (list)
	{
		if (list->data)
		{
			printf("freeing: [%s]\n", list->data);
			// ft_bzero(list->data, ft_strlen(list->data));
			free(list->data);
		}
		// list->prev = NULL;
		// free(list);
		// if (list->next)
		list = list->next;
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
