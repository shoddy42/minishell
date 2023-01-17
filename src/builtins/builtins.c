/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 20:31:43 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 19:49:45 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ms_exit(t_command *cmd)
{
	extern int	g_status;
	int			status;

	if (!cmd || !cmd->command || !cmd->command[1])
		exit (g_status);
	status = 0;
	if (ft_strisnum(cmd->command[1]) == false)
	{
		write(2, "exit: ", 7);
		write(2, cmd->command[1], ft_strlen(cmd->command[1]));
		write(2, ": numeric argument required\n", 29);
		exit (255);
	}
	if (cmd->command[2])
	{
		printf ("exit: Too many arguments.\n");
		return (1);
	}
	if (ft_strisnum(cmd->command[1]) == true)
		status = ft_atoi(cmd->command[1]);
	exit (status);
}

static void	builtin_redir(t_command *cmd)
{
	if (cmd->in_name)
	{
		cmd->infile = open(cmd->in_name, O_RDONLY);
		if (cmd->infile < 0)
			printf("invalid infile\n");
	}
	if (cmd->out_name)
	{
		if (cmd->outfile == O_TRUNC)
			cmd->outfile = open(cmd->out_name, O_WRONLY | O_CREAT | O_TRUNC);
		if (cmd->outfile == O_APPEND)
			cmd->outfile = open(cmd->out_name, O_WRONLY | O_CREAT | O_APPEND);
		if (cmd->outfile < 0)
			printf("invalid outfile\n");
	}
}

int	close_builtin(t_command *cmd)
{
	if (cmd->infile != STDIN_FILENO)
		close(cmd->infile);
	if (cmd->outfile != STDOUT_FILENO)
		close(cmd->outfile);
	return (0);
}

bool	is_builtin(t_command *cmd, t_minishell *shell)
{
	extern int	g_status;

	builtin_redir(cmd);
	if (!cmd->command || !cmd->command[0])
		return (false);
	if (ft_strcmp(cmd->command[0], "cd") == 0)
		g_status = ms_cd(cmd, shell);
	else if (ft_strcmp(cmd->command[0], "prompt") == 0)
		g_status = change_prompt(cmd, shell);
	else if (ft_strcmp(cmd->command[0], "echo") == 0)
		g_status = ms_echo(cmd);
	else if (ft_strcmp(cmd->command[0], "env") == 0)
		g_status = print_env(shell, cmd);
	else if (ft_strcmp(cmd->command[0], "pwd") == 0)
		g_status = ms_pwd(shell);
	else if (ft_strcmp(cmd->command[0], "export") == 0)
		g_status = ms_export(cmd, shell);
	else if (ft_strcmp(cmd->command[0], "unset") == 0)
		g_status = ms_unset(shell, cmd);
	else if (ft_strcmp(cmd->command[0], "exit") == 0)
		g_status = ms_exit(cmd);
	else
		return (false);
	return (true);
}
