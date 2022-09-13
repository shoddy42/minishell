/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 00:57:02 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/14 01:15:25 by wkonings      ########   odam.nl         */
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
	while (*env[++i])
	{
		tmp->next = new_env(env[i]);
		tmp = tmp->next;
	}
}
