/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 00:57:02 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/14 03:10:37 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_minishell *shell)
{
	int i = 0;
	t_env *test;

	test = shell->env;
	printf("ENVP: ");
	while (test && test->next)
	{
		printf("[%s]\n", test->data);
		test = test->next;
	}
	if (test)
	{
		printf("[%s]\n", test->data);
	}

}

t_env	*new_env(char *data)
{
	t_env *new;

	new = ft_calloc(1, sizeof(t_env));
	new->data = ft_strdup(data);
	return (new);
}

void	init_env(t_minishell *shell, char  **env)
{
	int i;
	t_env	*tmp;

	i = 0;
	shell->env = new_env(env[i]);
	tmp = shell->env;
	while (env[++i])
	{
		tmp->next = new_env(env[i]);
		tmp = tmp->next;
	}
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5) != 0 && env[i + 1])
		i++;
	if (ft_strncmp(env[i], "PATH=", 5) != 0)
		exit (1);
	shell->path = ft_split(env[i] + 6, ':');
	// TEST FOR PRINTING ENV and PATH
	// printf("%s\n", env[i]);	
	// i = -1;
	// while (shell->path[++i])
	// 	printf("P: [%s]\n", shell->path[i]);
}
