/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 02:42:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/26 13:05:47 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// this function is not properly protected, might need to exit instead of return?
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
	while (cmd[++j] && cmd[j] != ' ')
		ret[i + j + 1] = cmd[j];
	ret[i] = '/';
	ret[i + j + 1] = '\0';
	return (ret);
}

/*
this function is way too basic. we'll need split
the forking to a different process most likely.
we also cant call this directly from the command generator,
since we need to check for pipe BEFORE.
*/

int		child_p_1(t_command *cmd, t_minishell *shell)
{
	char	*path;
	int		i;

	if (!cmd->command)
		exit(-1);
	if (cmd->infile == -42 || cmd->outfile == -42)
		printf ("FILE ERROR!!!\n");
	if (cmd->infile != STDIN_FILENO && cmd->infile != NEEDS_PIPE)
		dup2(cmd->infile, STDIN_FILENO);
	if (cmd->outfile != STDOUT_FILENO && cmd->outfile != NEEDS_PIPE)
		dup2(cmd->outfile, STDOUT_FILENO);
	if (check_builtin(cmd, shell, CHILD) == 0)
		return (42);
	i = 0;
	if (cmd->command && access(cmd->command[0], X_OK) == 0)
		execve(cmd->command[0], cmd->command, shell->envp);
	while (shell->path[i])
	{
		path = pipex_pathjoin(shell->path[i], cmd->command[0]);
		if (access(path, X_OK) == 0)
			execve(path, cmd->command, shell->envp);
		free(path);
		i++;
	}
	write(2, "minishell: ", 12);
	write(2, cmd->command[0], ft_strlen(cmd->command[0]));
	write(2, ": command not found\n", 20);
	// printf ("%s: command not found.\n", cmd->command[0]);
	return (0);
}

int		test_child(t_command *cmd, t_minishell *shell, pid_t child, int i)
{

	//printf ("\nCHILD(%i): cmd->tunnel[%i][%i]\n", i, cmd->infile, cmd->outfile);
	child_p_1(cmd, shell);
	return (0);
}

int		tunnel_fork(t_command *cmd, t_minishell *shell)
{
	//creation and laying of pipes if needed.
	pipe(cmd->tunnel);
	//printf ("created FDS [%i][%i]\n", cmd->tunnel[0], cmd->tunnel[1]);
	if (cmd->outfile == NEEDS_PIPE)
		cmd->outfile = cmd->tunnel[WRITE];
	if (cmd->next) 
		if (cmd->next->infile == NEEDS_PIPE)
			cmd->next->infile = cmd->tunnel[READ];

	// fork
	if ((cmd->pid = fork()) < 0)
		ms_error("Forking failed.\n", -43);
	// parent duties. need to close a ton of file descriptors. ALL non NEEDS_PIPE and stdin.s EXCEPT cmd->tunnel[READ]
	if (cmd->pid > 0)
	{
		if (cmd->tunnel[WRITE])
			close(cmd->tunnel[WRITE]);	
		if (cmd->outfile != STDOUT_FILENO && cmd->outfile != NEEDS_PIPE)
			close(cmd->outfile);
		if (cmd->infile != STDIN_FILENO && cmd->infile != NEEDS_PIPE)
			close(cmd->infile);
		if (cmd != shell->commands) //check its not the first command.
			close (cmd->prev->tunnel[READ]);
			// if (cmd->prev->tunnel[READ])
		if (!cmd->next)
			close(cmd->tunnel[READ]);
	}
	if (cmd->pid == 0)
		close (cmd->tunnel[READ]);
	return (cmd->pid);
}

void    execute_two_electric_boogaloo(t_minishell *shell)
{
	pid_t		*children;
	pid_t		pid;
	t_command	*cmd;
	int			i;
	int			status;
	
	i = 0;
	cmd = shell->commands;
	//printf ("STARTING EXECUTE\n\n\n");
	if (!cmd)
		return ;
	children = ft_calloc(shell->pipe_count + 1, sizeof(pid_t));
	if (!children)
		ms_error("Failed at allocating PIDs.", -1);
	while (i <= shell->pipe_count)
	{
		//printf ("tunnelfork call #%i\n", i);
		if (check_builtin(cmd, shell, MINISHELL) == 0)
		{	
			i++;
			if (!cmd->next)
				break;
			cmd = cmd->next;
		}
		children[i] = tunnel_fork(cmd, shell);
		if (children[i] == 0)
		{
			test_child(cmd, shell, children[i], i);
			exit (1);
		}
		if (!cmd->next)
			break;
		cmd = cmd->next;
		i++;
	}
	while (i >= 0)
	{
		// printf("WAITING FOR PROCESS\n");
		pid = waitpid((pid_t)0, &status, 0);
		// printf("PROCESS [%i] ENDED WITH CODE (%i)\n", pid, status);
		i--;
	}
	// printf ("SURVIVED\n");
}