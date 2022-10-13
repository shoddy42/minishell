/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:05:15 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/13 13:08:02 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*handle_left(t_token *token, t_minishell *shell, t_command *cmd)
{
	t_token *tmp;

	tmp = NULL;
	printf("\nentered handle_left on token [%s]\n", token->data);
	if (token->next)
		tmp = token->next;
	if (!tmp)
	{
		//todo: error function, or just use perror?
		printf("ERROR HANDLE_LEFT, NO TOKEN\n");
		return (token);
	}
	if (tmp->type == LEFT && tmp->next)
	{
		tmp = heredoc(tmp, shell);
		if (tmp->type == HEREDOC_FILE)
			printf("heredoc success?\n");
	}
	else if (tmp->type == COMMAND)
	{
		// handling relinking the linked list. might turn this into a function of itself. OR add it to free_tokens_til.
		if (token->prev)
			tmp->prev = token->prev;
		else
			shell->tokens = tmp;
		free_tokens_til(token, tmp);


		tmp->fd = open(tmp->data, O_RDONLY);
		if (tmp->fd == -1)
		{
			// need to somehow cancel the WHOLE command from here.
			perror("No such file or directory.\n");
			return (token);
		}
		tmp->type = INFILE;
	}
	printf("returned token [%s]\n", tmp->data);
	cmd->infile = tmp->fd;
	printf ("cmd->infile = [%i]\n", cmd->infile);
	return (tmp);
}

// todo: add check for >>> and possibly other forms of wrong input.
// todo: split function so it's norme

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
		close (cmd->infile);
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