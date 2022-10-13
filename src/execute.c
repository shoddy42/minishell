/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 02:42:24 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/13 17:58:13 by wkonings      ########   odam.nl         */
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

// // this function probably needs to get overhauled
// char	**get_command_options(t_token	*token)
// {
// 	char	**command;
// 	t_token	*tmp;
// 	int		i;

// 	tmp = token;
// 	i = 0;
// 	while (tmp && (tmp->type == COMMAND || tmp->type == VOID))
// 	{
// 		if (tmp->type == COMMAND)
// 			i++;
// 		tmp = tmp->next;
// 	}
// 	command = ft_calloc(i + 1, sizeof(char *));
// 	i = 0;
// 	while (token && (token->type == COMMAND || token->type == VOID))
// 	{
// 		if (token->type == COMMAND)
// 		{
// 			command[i] = ft_strdup(token->data);
// 			i++;
// 		}
// 		token = token->next;
// 	}
// 	return (command);
// }

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

	// pre execution printing of all op the command options
	printf ("\nEXECUTION STARTS\n");

	printf("infile: %i\n", cmd->infile);
	printf("outfile: %i\n", cmd->outfile);
	int k = -1;
	while (cmd->command[++k] && child == 0)
		printf("opt: [%s]\n", cmd->command[k]);

	// actual code starts
	i = 0;
	child = fork();
	pipe(tunnel);
	if (child == 0)
		dup2(cmd->outfile, STDOUT_FILENO);
	if (cmd->command && access(cmd->command[0], X_OK) == 0 && child == 0)
		execve(cmd->command[0], cmd->command, shell->envp);
	while (shell->path[i] && child == 0)
	{
		path = pipex_pathjoin(shell->path[i], cmd->command[0]);
		if (access(path, X_OK) == 0)
			execve(path, cmd->command, shell->envp);
		free(path);
		i++;
	}
	if (child == 0)
	{
		printf("%s: command not found.\n", cmd->command[0]);
		exit(1);
	}
	waitpid(child, NULL, 0);
	if (check_builtin(cmd) == 1) //cmd->command doesnt give me the full command line, only "echo" but not anything after?
		;
		// printf("not gud mens");
}