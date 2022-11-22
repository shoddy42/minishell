/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commands.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 11:29:32 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/22 20:50:31 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//later: remove
// void	print_commands(t_minishell *shell)
// {
// 	t_command	*tmp;
// 	int			cmd_num;	
// 	int			i;

// 	i = 0;
// 	cmd_num = 0;
// 	tmp = shell->cmd_head;
// 	printf ("\nPrinting all commands:\n");
// 	while (tmp)
// 	{
// 		printf ("\nCMD (%i) infile [%i] outfile [%i]\n",
// 			cmd_num, tmp->infile, tmp->outfile);
// 		cmd_num++;
// 		i = -1;
// 		while (tmp && tmp->command && tmp->command[++i])
// 			printf("cmd(%i) = [%s]\n", i, tmp->command[i]);
// 		if (!tmp->next)
// 		{
// 			printf ("no command next\n");
// 			break ;
// 		}
// 		tmp = tmp->next;
// 	}
// }

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

t_token	*get_command_options(t_token *start, t_command *cmd)
{
	t_token	*token;
	int		i;

	token = start;
	i = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == COMMAND)
			i++;
		token = token->next;
	}
	cmd->command = ft_calloc(i + 1, sizeof(char *));
	token = start;
	i = 0;
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
			cmd = new_command(shell, cmd);
			if (cmd->infile == STDIN_FILENO)
				cmd->infile = NEEDS_PIPE;
			if (cmd->prev->outfile == STDOUT_FILENO)
				cmd->prev->outfile = NEEDS_PIPE;
		}
		token = token->next;
	}
	return (0);
}

		// else if (token && token->type == SEMICOLON) //only useful for semi
		// 	cmd = new_command(shell, cmd);