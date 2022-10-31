/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commands.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 11:29:32 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/31 16:45:39 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_commands(t_minishell *shell)
{
	t_command	*tmp;
	int	cmd_num = 0;
	int i = 0;

	tmp = shell->commands;
	printf ("\nPrinting all commands:\n");
	while(tmp)
	{
		printf ("\nCOMMAND (%i)    infile [%i] outfile [%i]\n", cmd_num, tmp->infile, tmp->outfile);
		cmd_num++;
		i = 0;
		while (tmp && tmp->command && tmp->command[i])
		{
			printf("cmd(%i) = [%s]\n", i, tmp->command[i]);
			i++;
		}
		if (!tmp->next)
		{
			printf ("no command next\n");
			break;
		}
		tmp = tmp->next;
	}
}

t_token	*get_command_options(t_token *token, t_command *cmd)
{
	t_token		*tmp;
	int			i;

	tmp = token;
	i = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == COMMAND)
			i++;
		tmp = tmp->next;
	}
	cmd->command = ft_calloc(i + 1, sizeof(char *));
	tmp = token;
	i = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == COMMAND)
			cmd->command[i++] = ft_strdup(tmp->data);
		if (tmp->type == INFILE || tmp->type == HEREDOC_FILE)
			cmd->infile = tmp->fd;
		if (tmp->type == OUTFILE)
			cmd->outfile = tmp->fd;
		if (!tmp->next)
			break;
		tmp = tmp->next;
	}
	return (tmp);
}

t_command	*new_command(t_minishell *shell, t_command *cmd)
{
	t_command	*new;

	new = ft_calloc(1, sizeof(t_command));
	if (!new)
		ms_error("Failed at allocating command.", -1, FALSE, shell);
	new->infile = STDIN_FILENO;
	new->outfile = STDOUT_FILENO;
	if (cmd != NULL)
	{
		new->prev = cmd;
		cmd->next = new;
	}
	return (new);
}

//todo: FREE the commands after use.
int make_commands(t_minishell *shell)
{
	t_command	*cmd;
	t_token		*token;
	int			i;

	cmd = new_command(shell, NULL);
	shell->commands = cmd;
	token = shell->tokens;
	// printf ("token? [%s]\n", token->data);
	// while (token && shell->cancel_command_line == FALSE)
	// {
	// 	token = get_command_options(token, cmd);
	// 	// //printf("returned token = [%s]\n", token->data);
	// 	i = 0;
	// 	if (token && token->type == PIPE)
	// 	{
	// 		cmd = new_command(shell, cmd);
	// 		if (cmd->infile == STDIN_FILENO)
	// 			cmd->infile = NEEDS_PIPE;
	// 		if (cmd->prev->outfile == STDOUT_FILENO)
	// 			cmd->prev->outfile = NEEDS_PIPE;
	// 	}
	// 	token = token->next;
	// }
	return (0);
}