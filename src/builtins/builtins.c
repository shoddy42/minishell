/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 20:31:43 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/18 20:20:44 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int	ms_exit(t_command *cmd, t_minishell *shell)
{
	int	status;

	status = 0;
	if (cmd && cmd->command && cmd->command[1])
	{
		if (cmd->command[2])
		{
			printf ("exit: Too many arguments.\n");
			return (1);
		}
		if (ft_strisnum(cmd->command[1]) == true)
			status = ft_atoi(cmd->command[1]);
		else
		{
			write(2, "Minishell: ", 12);
			write(2, cmd->command[1], ft_strlen(cmd->command[0]));
			write(2, ": numeric argument required\n", 29);
			status = 255;
		}
	}
	exit (shell->last_return);
}

void	builtin_redir(t_command *cmd, t_minishell *shell)
{
	if (cmd->in_name)
		cmd->infile = open(cmd->in_name, O_RDONLY);
	if (cmd->out_name)
		cmd->outfile = open(cmd->out_name, O_WRONLY);
	if (cmd->infile < 0 || cmd->outfile < 0)
		printf ("someting wong\n");
}

bool	is_builtin(t_command *cmd, t_minishell *shell)
{
	builtin_redir(cmd, shell);
	if (!cmd->command || !cmd->command[0])
		return (false);
	if (ft_strcmp(cmd->command[0], "cd") == 0)
		shell->last_return = ms_cd(cmd, shell);
	else if (ft_strcmp(cmd->command[0], "echo") == 0)
		shell->last_return = ms_echo(cmd);
	else if (ft_strcmp(cmd->command[0], "env") == 0)
		shell->last_return = print_env(shell, cmd);
	else if (ft_strcmp(cmd->command[0], "pwd") == 0)
		shell->last_return = ms_pwd(cmd);
	else if (ft_strcmp(cmd->command[0], "export") == 0)
		shell->last_return = ms_export(cmd, shell);
	else if (ft_strcmp(cmd->command[0], "unset") == 0)
		shell->last_return = ms_unset(shell, cmd);
	else if (ft_strcmp(cmd->command[0], "exit") == 0)
		shell->last_return = ms_exit(cmd, shell);
	else
		return (false);
	return (true);
}

int	ms_strchr(const char *src, int c)
{
	int		i;

	i = 0;
	while (src[i])
	{
		if (src[i] == (char)c)
			return (i);
		i++;
	}
	if (src[i] == (char)c)
		return (i);
	return (0);
}
