/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 02:42:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/21 13:36:41 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// this function is not properly protected, might need to exit instead of return?
// todo: STEAL THE LOGIC FROM PIPEX AGAIN TO MAKE THIS WORK BETTER!
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

// basic logic, probably needs double checking.

void	cmd_child_reopen(t_command *cmd, t_minishell *shell)
{
	if (!cmd->command)
		exit(-1);
	if (cmd->in_name)
	{
		printf ("opening infile [%s]\n", cmd->in_name);
		cmd->infile = open(cmd->in_name, O_RDONLY);
	}
	if (cmd->out_name)
	{
		printf ("opening outfile [%s]\n", cmd->out_name);
		cmd->outfile = open(cmd->out_name, O_WRONLY);
		if (cmd->outfile < 0)
			printf(" OUT BAD \n");
	}
	if (cmd->infile == -42 || cmd->outfile == -42 || cmd->infile < 0 || cmd->outfile < 0)
		cmd->executable = false;
	if (cmd->infile != STDIN_FILENO && cmd->infile != NEEDS_PIPE)
		if (dup2(cmd->infile, STDIN_FILENO) == -1)
			cmd->executable = false;
	if (cmd->outfile != STDOUT_FILENO && cmd->outfile != NEEDS_PIPE)
		if (dup2(cmd->outfile, STDOUT_FILENO) == -1)
			cmd->executable = false;
	if (cmd->executable == false)
		exit(1);
}

static bool	handle_path(char **args, char **envp)
{
	int		i;

	if (access(args[0], X_OK) == 0)
		if (execve(args[0], args, envp) == -1)
			exit (1);
	i = -1;
	while (args[0][++i])
		if (args[0][i] == '/')
			return (true);
	return (false);
}

void	cmd_execute(t_command *cmd, t_minishell *shell)
{
	char	*path;
	int		i;

	cmd_child_reopen(cmd, shell);
	if (is_builtin(cmd, shell) == true)
		exit(0); //change this?
	if (access(cmd->command[0], X_OK) == 0)
		if (execve(cmd->command[0], cmd->command, shell->envp) < 0)
			ms_error("error", 0, false, shell);
	i = -1;
	if (handle_path(cmd->command, shell->envp))
		exit(1); //todo: ERROR; no such file or msth
	while (shell->path && shell->path[++i])
	{
		path = pipex_pathjoin(shell->path[i], cmd->command[0]);
		if (access(path, X_OK) == 0)
			if (execve(path, cmd->command, shell->envp) < 0)
				ms_error("error", 0, false, shell);
		free(path);
	}
	path = pipex_pathjoin(".", cmd->command[0]);
	if (access(path, X_OK) == 0)
		if (execve(path, cmd->command, shell->envp) == -1)
			exit (1);
	free (path);
	write(2, "minishell: ", 12);
	write(2, cmd->command[0], ft_strlen(cmd->command[0]));
	write(2, ": command not found\n", 20);
	exit (127);
}

// pipe creating and closing algorithm seems right.
// remove return value?
int		tunnel_fork(t_command *cmd, t_minishell *shell)
{
	//creation and laying of pipes if needed.
	// if (cmd->executable == false)
	// 	return (0);
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
		signal(SIGINT, child_sig);
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
	{
		signal(SIGQUIT, child_sig);
		close (cmd->tunnel[READ]);
	}
	return (cmd->pid);
}


void    execute_two_electric_boogaloo(t_minishell *shell)
{
	t_command	*cmd;
	pid_t		pid;
	int			i;
	int			status;
	int			last_status;
	
	
	status = 0;
	last_status = -42;
	cmd = shell->commands;
	if (!cmd)
	{
		printf ("no cmd!\n"); //todo: real error.
		return ;
	}
	if (cmd && !cmd->next && cmd->executable == true)
		if (is_builtin(cmd, shell) == true)
			return ;
	// creation of shell->envp
	// printf ("PRINTING ENVP\n");
	// create_envp(shell);
	// print_envp(shell->envp);

	
	// pipeline logic.
	i = 0;
	while (i <= shell->pipe_count)
	{
		tunnel_fork(cmd, shell);
		i++;
		if (cmd->pid == 0)
			cmd_execute(cmd, shell);
		if (!cmd->next)
			break;
		cmd = cmd->next;
	}
	shell->last_cmd = cmd->pid;
	//gathering of latest exit status.
	while (i > 0)
	{
		// printf("WAITING FOR [%i] PROCESS\n", i);
		pid = waitpid((pid_t)0, &status, 0);
		if (pid == shell->last_cmd)
		{
			if (WIFEXITED(status))
			{
				last_status = WEXITSTATUS(status);
				// printf ("Last CMD Status [%i]\n", last_status);
			}
		}
		if (WIFEXITED(status))
		{
			shell->last_return = WEXITSTATUS(status);
			// printf ("Status [%i]\n", last_status);

		}
		else
			shell->last_return = 258;
		// printf("PROCESS [%i] ENDED WITH CODE:(%i) STATUS:(%i)\n", pid, status, WEXITSTATUS(status));
		i--;
	}
	if (last_status != -42)
		shell->last_return = last_status;
	// printf ("LAST Status [%i]\n", shell->last_return);
	
}
