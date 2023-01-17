/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 11:26:52 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 15:45:33 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	remove_list(t_env	*env)
{
	if (env->beans)
		free(env->beans);
	if (env->data)
		free(env->data);
	if (env->key)
		free(env->key);
	env->beans = NULL;
	env->data = NULL;
	env->key = NULL;
	free(env);
}

void	remove_env(t_env *env, char *key, t_minishell *shell)
{
	t_env	*prev;

	prev = shell->env_head;
	if (!env || !key || !prev)
		return ;
	while (prev && prev->next)
	{
		if (prev->next == env && env->next)
			prev->next = env->next;
		else if (prev->next == env)
			prev->next = NULL;
		prev = prev->next;
	}
	remove_list(env);
}

int	ms_unset(t_minishell *shell, t_command *cmd)
{
	int		i;
	t_env	*env;

	env = shell->env_head;
	i = 0;
	while (cmd->command[++i])
	{
		env = env_exists(cmd->command[i], shell);
		if (env)
			remove_env(env, cmd->command[i], shell);
	}
	create_envp(shell);
	return (0);
}
