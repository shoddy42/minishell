/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/22 18:52:21 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 18:04:38 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parent_close(t_command *cmd, t_minishell *shell)
{
	signal(SIGINT, child_sig);
	signal(SIGQUIT, child_sig);
	if (cmd->tunnel[WRITE])
		close(cmd->tunnel[WRITE]);
	if (cmd->outfile != STDOUT_FILENO && cmd->outfile != NEEDS_PIPE)
		close(cmd->outfile);
	if (cmd->infile != STDIN_FILENO && cmd->infile != NEEDS_PIPE)
		close(cmd->infile);
	if (cmd != shell->cmd_head)
		close(cmd->prev->tunnel[READ]);
	if (!cmd->next)
		close(cmd->tunnel[READ]);
}

// check what ahppen if mallok fail
char	*pipex_pathjoin(char const *path, char const *cmd)
{
	char	*ret;
	size_t	i;
	size_t	j;

	if (!path || !cmd)
		return (NULL);
	ret = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(cmd) + 2));
	if (!ret)
		return (NULL);
	i = -1;
	j = -1;
	while (path[++i])
		ret[i] = path[i];
	while (cmd[++j])
		ret[i + j + 1] = cmd[j];
	ret[i] = '/';
	ret[i + j + 1] = '\0';
	return (ret);
}

bool	handle_path(char **args, char **envp)
{
	int			i;

	if (access(args[0], X_OK) == 0)
		execve(args[0], args, envp);
	i = -1;
	while (args[0][++i])
		if (args[0][i] == '/')
			return (true);
	return (false);
}

void	local_command(char **args, char **envp)
{
	char	*path;

	if (ft_strncmp(args[0], "./", 2) == 0)
	{
		perror(args[0]);
		exit (127);
	}
	path = pipex_pathjoin(".", args[0]);
	if (!path)
		return ;
	if (access(path, F_OK) < 0)
		return ;
	free(args[0]);
	args[0] = path;
	if (access(path, X_OK) == 0)
		execve(args[0], args, envp);
	if (access(path, F_OK) == 0)
	{
		perror(args[0]);
		free (path);
		exit (126);
	}
	free (path);
}

//OLD DEBUG MESSAGES
// printf("WAITING FOR [%cmd_count] PROCESS\n", cmd_count);
// printf("PROCESS [%cmd_count] ENDED WITH CODE:(%cmd_count)
// STATUS:(%cmd_count)\n", pid, status, WEXITSTATUS(status));
void	set_status(t_minishell *shell, int cmd_count)
{
	pid_t		pid;
	int			status;
	extern int	g_status;
	int			last_status;

	status = 0;
	last_status = -42;
	while (cmd_count > 0)
	{
		pid = waitpid((pid_t)0, &status, 0);
		if (pid == shell->last_cmd)
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		cmd_count--;
	}
	if (last_status != -42)
		g_status = last_status;
}
