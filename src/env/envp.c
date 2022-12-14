/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   envp.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/22 19:13:40 by wkonings      #+#    #+#                 */
/*   Updated: 2022/12/14 17:21:31 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	create_path(t_minishell *shell)
{
	int		i;

	i = -1;
	if (shell->path)
		while (shell->path[++i])
			free(shell->path[i]);
	if (shell->path)
	{
		free(shell->path);
		shell->path = NULL;
	}
	i = -1;
	while (shell->envp[++i])
		if (ft_strncmp("PATH=", shell->envp[i], 5) == 0)
			shell->path = ft_split(shell->envp[i] + 5, ':');
}

void	create_envp(t_minishell *shell)
{
	t_env	*env;
	int		size;
	int		i;

	size = 0;
	env = shell->env_head;
	while (env)
	{
		size++;
		env = env->next;
	}
	env = shell->env_head;
	if (shell->envp)
		free(shell->envp);
	shell->envp = ft_calloc(size + 1, sizeof(char *));
	if (!shell->envp || !env)
		printf ("major error\n");
	i = 0;
	while (env)
	{
		shell->envp[i++] = env->beans;
		env = env->next;
	}
	create_path(shell);
}
