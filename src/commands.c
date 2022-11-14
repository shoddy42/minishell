/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commands.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 11:29:32 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/14 11:06:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_commands(t_minishell *shell)
{
	t_command	*tmp;
	int			cmd_num;	
	int			i;

	i = 0;
	cmd_num = 0;
	tmp = shell->commands;
	printf ("\nPrinting all commands:\n");
	while (tmp)
	{
		printf ("\nCMD (%i) infile [%i] outfile [%i]\n",
			cmd_num, tmp->infile, tmp->outfile);
		cmd_num++;
		i = -1;
		while (tmp && tmp->command && tmp->command[++i])
			printf("cmd(%i) = [%s]\n", i, tmp->command[i]);
		if (!tmp->next)
		{
			printf ("no command next\n");
			break ;
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
	while (tmp && tmp->type != PIPE) // && tmp->type != SEMICOLON)
	{
		if (tmp->type == COMMAND)
			i++;
		tmp = tmp->next;
	}
	cmd->command = ft_calloc(i + 1, sizeof(char *));
	tmp = token;
	i = 0;
	while (tmp && tmp->type != PIPE) // && tmp->type != SEMICOLON)
	{
		if (tmp->type == COMMAND)
			cmd->command[i++] = ft_strdup(tmp->data);
		if (tmp->type == INFILE || tmp->type == HEREDOC_FILE)
			cmd->infile = tmp->fd;
		if (tmp->type == OUTFILE)
			cmd->outfile = tmp->fd;
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}
	// if (cmd->executable == false)
	// 	printf ("error found, cancelling command [%s]\n", cmd->command[0]);

t_command	*new_command(t_minishell *shell, t_command *cmd)
{
	t_command	*new;

	new = ft_calloc(1, sizeof(t_command));
	if (!new)
		ms_error("Failed at allocating command.", -1, true, shell);
	new->infile = STDIN_FILENO;
	new->outfile = STDOUT_FILENO;
	new->executable = true;
	if (cmd != NULL)
	{
		new->prev = cmd;
		cmd->next = new;
	}
	return (new);
}

int	make_commands(t_minishell *shell)
{
	t_command	*cmd;
	t_token		*token;

	cmd = new_command(shell, NULL);
	shell->commands = cmd;
	token = shell->tokens;
	if (!token)
	{
		cmd->executable = false;
		return (1);
	}
	// printf ("token? [%s]\n", token->data);
	while (token)
	{
		token = get_command_options(token, cmd);
		// printf("returned token = [%s]\n", token->data);
		if (token && token->type == PIPE)
		{
			cmd = new_command(shell, cmd);
			if (cmd->infile == STDIN_FILENO)
				cmd->infile = NEEDS_PIPE;
			if (cmd->prev->outfile == STDOUT_FILENO)
				cmd->prev->outfile = NEEDS_PIPE;
		}
		else if (token && token->type == SEMICOLON)
			cmd = new_command(shell, cmd);
		token = token->next;
	}
	return (0);
}
