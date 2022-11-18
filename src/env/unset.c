/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 11:26:52 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/18 22:40:18 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	remove_list(t_env	*env)
{
	if (env->beans)
	{
		free(env->beans);
		env->beans = NULL;
	}
	if (env->key)
	{
		free(env->key);
		env->key = NULL;
	}
	if (env->data)
	{
		free(env->data);
		env->data = NULL;
	}
	free(env);
	return (0);
}

int	ms_unset_loop(t_minishell *shell, char *cmd, t_env *tmp)
{
	t_env	*tmp1;
	t_env	*tmp2;

	printf("HERE\n");
	if (!cmd || !tmp)
		return (1);
	if (tmp->next)
		printf("TMPS NEXT: %s\n", tmp->next->key);
	if (tmp->next)
		tmp2 = tmp->next;
	tmp->next = tmp2->next;
	remove_list(tmp2);
	return (0);
}

int	ms_unset(t_minishell *shell, t_command *cmd)
{
	int		i;
	t_env	*tmp;

	tmp = shell->env;
	if (!tmp)
		return (1);
	if (ft_strcmp(tmp->key, cmd->command[i]) == 0)
	{
		if (tmp->next)
			shell->env = tmp->next;
		return (remove_list(tmp));
	}
	i = 0;
	while (cmd->command[++i])
	{
		while (tmp && tmp->next)
		{
			if (ft_strcmp(cmd->command[i], tmp->next->key) == 0)
				ms_unset_loop(shell, cmd->command[i], tmp);
			tmp = tmp->next;
		}
		tmp = shell->env;
	}
	return (0);
}
