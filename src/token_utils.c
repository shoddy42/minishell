/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:32:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/01 15:46:38 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// REMOVE THIS FUNCTION LATER
void	print_tokens(t_minishell *shell)
{
	int i = 0;
	t_token *test;

	test = shell->tokens;
	printf("\nall tokens: ");
	while (test && test->next)
	{
		if (test->type)// && test->type != VOID)
		{
			printf("(%s)", print_token_type(test->type));
			printf("[%s]-", test->data);
		}
		test = test->next;
	}
	if (test != NULL)
	{
		if (test->type)// && test->type != VOID)
		{
			// printf("final token ");
			printf("(%s)", print_token_type(test->type));
			printf("[%s]\n", test->data);
		}
	}
	printf("\n");
}

//	remove later
//	to check if token->prev is still fully linked.
void	print_tokens_backwards(t_minishell *shell)
{
	int i = 0;
	t_token *test;

	test = shell->tokens;
	printf("backwards:  ");
	while (test && test->next)
		test = test->next;
	while (test && test->prev)
	{
		if (test->type && test->type != VOID)
		{
			printf("(%s)", print_token_type(test->type));
			printf("[%s]-", test->data);
		}
		test = test->prev;
	}
	if (test != NULL)
	{
		if (test->type && test->type != VOID)
		{
			// printf("final token ");
			printf("(%s)", print_token_type(test->type));
			printf("[%s]\n", test->data);
		}
	}
	printf("\n");
}

char *print_token_type(int type)
{
	if (type == COMMAND)
		return "COMMAND";
	if (type == QUOTE)
		return "QUOTE";
	if (type == DQUOTE)
		return "DQUOTE";
	if (type == VARIABLE)
		return "VARIABLE";
	if (type == AND)
		return "AND";
	if (type == LEFT)
		return "LEFT";
	if (type == RIGHT)
		return "RIGHT";
	if (type == PIPE)
		return "PIPE";
	if (type == HEREDOC)
		return "HEREDOC";
	if (type == HEREDOC_FILE)
		return "HEREDOC_FILE";
	if (type == OUTFILE)
		return "OUTFILE";
	if (type == INFILE)
		return "INFILE";
	if (type ==	ERROR)
		return "ERROR";
	if (type == VOID)
		return "VOID";
	return ("TYPELESS ERROR!!!");
}

// maybe rename to REMOVE_tokens_til?
// free tokens starting from START, up to END, does NOT free END. IF there is a prev token, it will relink. so far it only links  start->prev to end 
void	free_tokens_til(t_token *start, t_token *end)
{
	t_token	*tmp;
	t_token	*replace_prev;

	if (start->prev)
	{
		start->prev->next = end;
		replace_prev = start->prev;
	}
	while (start != end)
	{
		tmp = start;
		start = start->next;
		if (tmp->data)
			free(tmp->data);
		free(tmp);
	}
	end->prev = replace_prev;
}

void	free_single_token(t_token *token)
{
	if (token->prev && token->next)
	{
		printf ("ESTABLISHING CONNECTION\n");
		token->prev->next = token->next;
		token->next->prev = token->prev;
	}
	else if (!token->next && token->prev)
		token->prev->next = NULL;
	else if (!token->prev && token->next)
		token->next->prev = NULL;
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
		// printf ("freeing token [%s] at adress [%p]\n", list->data, &list->data);
		if (list->data)
			free(list->data);
		list = list->next;
		if (list->prev)
			free(list->prev);
	}
	if (list)
	{
		// printf ("freeing token [%s] at adress [%p]\n", list->data, &list->data);
		if (list->data)
			free(list->data);
		free(list);
	}
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
