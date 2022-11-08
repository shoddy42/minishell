/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 02:42:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/08 21:34:48 by wkonings      ########   odam.nl         */
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
	while (cmd[++j]) //while (cmd[++j] && cmd[j] != ' ') old loop 
		ret[i + j + 1] = cmd[j];
	ret[i] = '/';
	ret[i + j + 1] = '\0';
	return (ret);
}

// basic logic, probably needs double checking.
//todo: protect dup2s, if one fails cancel its command.
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
	char *str;

	// printf ("\nCHILD(%i): cmd->tunnel[%i][%i]\n", i, cmd->infile, cmd->outfile);
	// printf("ENTERED CHILD\n");
	// printf ("cmd: fd_in[%i] \n", cmd->infile);
	child_p_1(cmd, shell);
	return (0);
}

// pipe creating and closing algorithm seems right.
// remove return value?
int		tunnel_fork(t_command *cmd, t_minishell *shell)
{
	//creation and laying of pipes if needed.
	if (pipe(cmd->tunnel) < 0)
		cmd->executable = false;
	if (cmd->outfile == NEEDS_PIPE)
		cmd->outfile = cmd->tunnel[WRITE];
	if (cmd->next) 
		if (cmd->next->infile == NEEDS_PIPE)
			cmd->next->infile = cmd->tunnel[READ];


	// fork
	if ((cmd->pid = fork()) < 0)
		ms_error("Forking failed.\n", -43, false, shell);


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
			close(cmd->prev->tunnel[READ]);
		if (!cmd->next)
			close(cmd->tunnel[READ]);
	}
	if (cmd->pid == 0)
		close (cmd->tunnel[READ]);
	return (cmd->pid);
}

//todo: change the cancel command line to only cancel one of the commands, using cmd->executable
void    execute_two_electric_boogaloo(t_minishell *shell)
{
	t_command	*cmd;
	pid_t		pid;
	int			i;
	int			status;
	int			last_status;
	
	i = 0;
	if (shell->cancel_command_line == true)
		return ;
	cmd = shell->commands;
	if (!cmd)
		return ;

	if (cmd && !cmd->next)
		if (check_builtin(cmd, shell, MINISHELL) == 0)
			shell->pipe_count--;
	// pipeline logic.
	while (i <= shell->pipe_count)
	{
		tunnel_fork(cmd, shell);
		if (cmd->pid == 0)
		{
			test_child(cmd, shell, cmd->pid, i);
			exit (1);
		}
		if (!cmd->next)
		{
			shell->last_cmd = cmd->pid;
			break;
		}
		cmd = cmd->next;
		i++;
	}
	//gathering of latest exit status.
	while (i >= 0)
	{
		// printf("WAITING FOR PROCESS\n");
		pid = waitpid((pid_t)0, &status, 0);
		if (pid == shell->last_cmd)
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		if (WIFEXITED(status))
			shell->last_return = WEXITSTATUS(status);
		else
			shell->last_return = -69;
		// printf("PROCESS [%i] ENDED WITH CODE:(%i) STATUS:(%i)\n", pid, status, WEXITSTATUS(status));
		i--;
	}
	shell->last_return = last_status;
}