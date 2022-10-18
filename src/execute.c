/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 02:42:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/18 21:58:37 by wkonings      ########   odam.nl         */
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

// void    execute(t_command *cmd, t_minishell *shell)
// {
// 	int		i;
// 	char	*path;
// 	pid_t	child;
// 	int		tunnel[2];

// 	// pre execution printing of all op the command options
// 	printf ("\nEXECUTION STARTS\n");

// 	printf("infile: %i\n", cmd->infile);
// 	printf("outfile: %i\n", cmd->outfile);
// 	int k = -1;
// 	while (cmd->command[++k] && child == 0)
// 		printf("opt: [%s]\n", cmd->command[k]);

// 	// actual code starts
// 	i = 0;
// 	if (check_builtin(cmd, shell) == 0)
// 		return ; //cmd->command doesnt give me the full command line, only "echo" but not anything after?
// 	child = fork();
// 	pipe(tunnel);
// 	if (child == 0)
// 		dup2(cmd->outfile, STDOUT_FILENO);
// 	if (cmd->command && access(cmd->command[0], X_OK) == 0 && child == 0)
// 		execve(cmd->command[0], cmd->command, shell->envp);
// 	while (shell->path[i] && child == 0)
// 	{
// 		path = pipex_pathjoin(shell->path[i], cmd->command[0]);
// 		if (access(path, X_OK) == 0)
// 			execve(path, cmd->command, shell->envp);
// 		free(path);
// 		i++;
// 	}
// 	if (child == 0)
// 	{
// 		printf("%s: command not found.\n", cmd->command[0]);
// 		exit(1);
// 	}
// 	waitpid(child, NULL, 0);
// 		;
// 		// printf("not gud mens");
// }

int		child_p_1(t_command *cmd, t_minishell *shell)
{
	char	*path;
	int		i;

	if (cmd->infile != STDIN_FILENO)
		dup2(cmd->infile, STDIN_FILENO);
	if (cmd->outfile != STDOUT_FILENO)
		dup2(cmd->outfile, STDOUT_FILENO);
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
	printf ("%s: command not found.\n", cmd->command[0]);
	return (0);
}

int		test_child(t_command *cmd, t_minishell *shell, pid_t child)
{

	printf ("\nCHILD(%i): tunnel[%i][%i]\n", child, cmd->infile, cmd->outfile);
	child_p_1(cmd, shell);
	close(cmd->infile);
	close(cmd->outfile);

	return (0);
}

void    execute_two_electric_boogaloo(t_minishell *shell)
{
	pid_t		*children;
	pid_t		pid;
	t_command	*cmd;
	int			i;
	int			status = 0;
	
	i = 0;
	cmd = shell->commands;
	children = ft_calloc(shell->pipe_count + 1, sizeof(pid_t));
	while (i <= shell->pipe_count)
	{
		if ((children[i] = fork()) < 0)
		{
			printf("FORKING ERROR\n");
			exit (66);
		}
		else if (children[i] == 0)
		{
			test_child(cmd, shell, children[i]);
			exit (1);
		}
		if (children[i] > 0)
		{
			printf ("closing fd's in parent\n");
			if (cmd->infile != STDIN_FILENO)
				close(cmd->infile);
			if (cmd->outfile != STDOUT_FILENO)
				close(cmd->outfile);
		}
		if (!cmd->next)
			break;
		cmd = cmd->next;
		i++;
	}
	while (i >= 0)
	{
		printf("WAITING FOR PROCESS\n");
		pid = waitpid((pid_t)0, &status, 0);
		printf("PROCESS [%i] ENDED WITH CODE (%i)\n", pid, status);
		i--;
	}
	printf ("SURVIVED\n");
}