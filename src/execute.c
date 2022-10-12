/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 02:42:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/12 21:01:13 by auzochuk      ########   odam.nl         */
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

void    execute(t_command *cmd, t_minishell *shell)
{
	int		i;
	char	*path;
	pid_t	child;
	int		tunnel[2];

	child = fork();
	i = 0;

	int k = -1;
	while (cmd->options[++k] && child == 0)
		printf("opt: [%s]\n", cmd->options[k]);


	pipe(tunnel);
	if (child == 0)
		dup2(cmd->outfile, STDOUT_FILENO);
	if (cmd->options && access(cmd->options[0], X_OK) == 0 && child == 0)
		execve(cmd->options[0], cmd->options, shell->envp);
	while (shell->path[i] && child == 0)
	{
		path = pipex_pathjoin(shell->path[i], cmd->command);
		if (access(path, X_OK) == 0)
		{
			// printf("its go time\n");
			execve(path, cmd->options, shell->envp);
		}
		free(path);
		i++;
	}
	if (child == 0)
	{
		printf("%s: command not found.\n", cmd->options[0]);
		exit(1);
	}
	waitpid(child, NULL, 0);
	if (check_builtin(cmd) == 1)
		;
		// printf("not gud mens");
}