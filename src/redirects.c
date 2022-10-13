/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:05:15 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/13 10:06:20 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*handle_left(t_token *token, t_minishell *shell)
{
	t_token *tmp;

	tmp = token;
	if (tmp->next && tmp->next->type == LEFT)
	{
		tmp = tmp->next;
		tmp = heredoc(tmp, shell);
		if (tmp->type == HEREDOC_FILE)
			printf("heredoc time\n");
	}
	return (token);
}

/*
	todo: add check for >>> and possibly other forms of wrong input.
	todo: split function so it's norme
*/

t_token	*handle_right(t_token *token, t_minishell *shell, t_command *cmd)
{
	t_token 	*tmp;
	int			append;

	append = 0;
	tmp = NULL;
	printf("\nentered handle_right on token [%s]\n", token->data);
	if (token->next)
		tmp = token->next;
	if (!tmp)
	{
		printf("ERROR HANDLE_RIGHT, NO TOKEN\n");
		return (token);
	}
	if (tmp->type == RIGHT)
	{
		append = 1;
		if (tmp->next)
			tmp = tmp->next;
	}
	while (tmp->type == VOID && tmp->next)
		tmp = tmp->next;
	if (tmp->type != COMMAND)
	{
		printf ("REDIRECT FAILURE, NO FILENAME GIVEN\n");
		return (token);
	}
	else
		printf ("opening file [%s]\n", tmp->data);
	if (append == 1)
	{
		printf ("OPENING IN APPEND MODE\n");
		cmd->outfile = open(tmp->data, O_RDWR | O_APPEND | O_CREAT, 0644);
	}
	else
		cmd->outfile = open(tmp->data, O_RDWR | O_TRUNC | O_CREAT, 0644);
	printf("cmd outfile fd = [%i]\n", cmd->outfile);
	tmp->type = OUTFILE;
	if (!token->prev) //ugly fix, but the problem is if token happens to be the HEAD (shell->tokens), we will segf.
	{
		printf ("NEW HEAD INSTALLED\n");
		shell->tokens = tmp;
	}
	free_tokens_til(token, tmp);
	return (tmp);
}