/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commands.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 11:29:32 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:53:18 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	set_command_options(t_token *token, t_command *cmd)
{
	if (token->type == INFILE || token->type == HEREDOC)
		cmd->in_name = token->data;
	if (token->type == OUTFILE)
	{
		cmd->out_name = token->data;
		cmd->outfile = token->fd;
	}
	if (token->type == ERROR)
		cmd->executable = false;
}

int	get_options_amount(t_token *token)
{
	int	i;

	i = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == COMMAND)
			i++;
		token = token->next;
	}
	return (i);
}

t_token	*get_command_options(t_token *start, t_command *cmd)
{
	t_token	*token;
	int		i;

	i = 0;
	token = start;
	cmd->command = ft_calloc(get_options_amount(token) + 1, sizeof(char *));
	if (!cmd->command)
		return (token_error(start, "error: command allocation failed ["));
	while (token && token->type != PIPE)
	{
		if (token->type == COMMAND)
			cmd->command[i++] = ft_strdup(token->data);
		set_command_options(token, cmd);
		if (!token->next)
			break ;
		token = token->next;
	}
	if (!cmd->command[0])
		cmd->executable = false;
	return (token);
}

t_command	*new_command(t_command *cmd)
{
	t_command	*new;

	new = ft_calloc(1, sizeof(t_command));
	if (!new)
		ms_error("Failed at allocating command.", -1, true);
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

	cmd = new_command(NULL);
	shell->cmd_head = cmd;
	token = shell->token_head;
	if (!token)
	{
		cmd->executable = false;
		return (1);
	}
	while (token)
	{
		token = get_command_options(token, cmd);
		if (token && token->type == PIPE)
		{
			cmd = new_command(cmd);
			if (cmd->infile == STDIN_FILENO)
				cmd->infile = NEEDS_PIPE;
			if (cmd->prev->outfile == STDOUT_FILENO)
				cmd->prev->outfile = NEEDS_PIPE;
		}
		token = token->next;
	}
	return (0);
}
