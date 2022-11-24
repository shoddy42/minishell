/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 02:42:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 17:42:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//todo: make sure there is only 1 global move all externs to in the funcs
void	cmd_child_open(t_command *cmd)
{
	if (cmd->in_name)
	{
		cmd->infile = open(cmd->in_name, O_RDONLY);
		if (cmd->infile < 0)
			printf ("OPENING IN FAILED\n");
	}
	if (cmd->out_name)
	{
		if (cmd->outfile == O_TRUNC)
			cmd->outfile = open(cmd->out_name, O_WRONLY | O_CREAT | O_TRUNC);
		if (cmd->outfile == O_APPEND)
			cmd->outfile = open(cmd->out_name, O_WRONLY | O_CREAT | O_APPEND);
		if (cmd->outfile < 0)
			printf(" OUT BAD \n");
	}
	if (cmd->infile == NEEDS_PIPE || cmd->outfile == NEEDS_PIPE
		|| cmd->infile < 0 || cmd->outfile < 0)
		cmd->executable = false;
}

// basic logic, probably needs double checking.
void	cmd_child_fd(t_command *cmd, t_minishell *shell)
{
	if (!cmd->command)
		exit(-1);
	cmd_child_open(cmd);
	if (cmd->infile != STDIN_FILENO && cmd->infile != NEEDS_PIPE)
		if (dup2(cmd->infile, STDIN_FILENO) == -1)
			cmd->executable = false;
	if (cmd->outfile != STDOUT_FILENO && cmd->outfile != NEEDS_PIPE)
		if (dup2(cmd->outfile, STDOUT_FILENO) == -1)
			cmd->executable = false;
	if (cmd->executable == false)
		exit(1);
}

void	cmd_execute(t_command *cmd, t_minishell *shell)
{
	char	*path;
	int		i;

	cmd_child_fd(cmd, shell);
	if (is_builtin(cmd, shell) == true)
		exit(0);
	i = -1;
	if (handle_path(cmd->command, shell->envp) == false)
	{
		while (shell->path && shell->path[++i])
		{
			path = pipex_pathjoin(shell->path[i], cmd->command[0]);
			if (access(path, X_OK) == 0)
				execve(path, cmd->command, shell->envp);
			free(path);
		}
	}
	local_command(cmd->command, shell->envp); // test!
	write(2, "minishell: ", 12);
	write(2, cmd->command[0], ft_strlen(cmd->command[0]));
	write(2, ": command not found\n", 20);
	exit (127);
}

// pipe creating and closing algorithm seems right.
// remove return value?
int	tunnel_fork(t_command *cmd, t_minishell *shell)
{
	if (pipe(cmd->tunnel) < 0)
		cmd->executable = false;
	if (cmd->outfile == NEEDS_PIPE)
		cmd->outfile = cmd->tunnel[WRITE];
	if (cmd->next)
		if (cmd->next->infile == NEEDS_PIPE)
			cmd->next->infile = cmd->tunnel[READ];
	cmd->pid = fork();
	if (cmd->pid < 0)
		ms_error("Forking failed.\n", -43, false, shell);
	if (cmd->pid > 0)
		parent_close(cmd, shell);
	if (cmd->pid == 0)
	{
		close (cmd->tunnel[READ]);
	}
	return (cmd->pid);
}

void	execute_two_electric_boogaloo(t_minishell *shell)
{
	t_command	*cmd;
	int			i;

	cmd = shell->cmd_head;
	if (!cmd)
		return ;
	if (cmd && !cmd->next && cmd->executable == true)
		if (is_builtin(cmd, shell) == true && close_builtin(cmd, shell) == 0)
			return ;
	i = 0;
	while (i++ <= shell->pipe_count)
	{
		tunnel_fork(cmd, shell);
		if (cmd->pid == 0)
			cmd_execute(cmd, shell);
		if (!cmd->next)
			break ;
		cmd = cmd->next;
	}
	shell->last_cmd = cmd->pid;
	set_status(shell, i);
}
