/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 20:32:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/22 19:18:19 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// REMOVE THIS FUNCTION LATER
void	print_tokens(t_minishell *shell)
{
	int i = 0;
	t_token *test;

	test = shell->token_head;
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
// void	print_tokens_backwards(t_minishell *shell)
// {
// 	int i = 0;
// 	t_token *test;

// 	test = shell->token_head;
// 	printf("backwards:  ");
// 	while (test && test->next)
// 		test = test->next;
// 	while (test && test->prev)
// 	{
// 		if (test->type && test->type != VOID)
// 		{
// 			printf("(%s)", print_token_type(test->type));
// 			printf("[%s]-", test->data);
// 		}
// 		test = test->prev;
// 	}
// 	if (test != NULL)
// 	{
// 		if (test->type && test->type != VOID)
// 		{
// 			printf("(%s)", print_token_type(test->type));
// 			printf("[%s]\n", test->data);
// 		}
// 	}
// 	printf("\n");
// }

// delete later
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

//todo: think about whether i want to preserve INFILE OUTFILE names
/**
 * @brief	Merges tokens into one, by appending all token->data from start to end.
 * 			This does cause a loss of FDs within tokens, so only use it for literals.
 * 
 * @param start The token to start appending from. Will get freed.
 * @param end 	The token to append up to.
 * @param shell The shell.
 * @returns		The passed token @b [end], after it's data has been appended. 
 * 
 * @bug			NOT YET TESTED
 */
t_token	*merge_tokens(t_token *start, t_token *end, t_minishell *shell)
{
	t_token	*token;
	char	*new_data;

	token = start;
	new_data = ft_strdup("");
	while (token && new_data)
	{
		new_data = ft_strexpand(new_data, token->data);
		if (token == end || !token->next)
			break ;
		token = token->next;
	}
	if (!new_data)
		return (token_error(start, "Token merge error. [", true));
	free (end->data);
	end->data = new_data;
	remove_tokens(start, end, shell);
	return (end);
}

/**
 * @brief	Frees all tokens from @b [START] up to @b [END]. 
 * 			Relinks tokens after deletion if needed.
 * 
 * @param start The first token to free.
 * 	 	If there is a start->prev, it will be linked to end->prev.
 * @param end 	The token to end on. This token does not get freed.
 * @param shell The shell.
 */
void	remove_tokens(t_token *start, t_token *end, t_minishell *shell)
{
	t_token	*tmp;
	t_token	*replace_prev;

	replace_prev = NULL;
	if (start->prev)
	{
		start->prev->next = end;
		replace_prev = start->prev;
	}
	else
		shell->token_head = end;
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

/**
 * @brief Frees a single token in the list, and relinks tokens when needed.
 * 
 * @param token The token to be freed.
 */
void	free_single_token(t_token *token)
{
	if (token->prev && token->next)
	{
		token->prev->next = token->next;
		token->next->prev = token->prev;
	}
	else if (!token->next && token->prev)
		token->prev->next = NULL;
	else if (!token->prev && token->next)
		token->next->prev = NULL;
	free(token->data);
	free(token);
}

/**
 * @brief	Frees all tokens and their data
 *  		starting from shell->token_head
 *			And sets shell->token_head to NULL
 * 
 * @param shell The shell.
 */
void	free_tokens(t_minishell *shell)
{
	t_token	*token;

	token = shell->token_head;
	while (token && token->next)
	{
		if (token->data)
			free(token->data);
		token = token->next;
		if (token->prev)
			free(token->prev);
	}
	if (token)
	{
		if (token->data)
			free(token->data);
		free(token);
	}
	shell->token_head = NULL;
}

t_token	*get_last_token(t_token *list)
{
	t_token	*tmp;

	tmp = list;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}
