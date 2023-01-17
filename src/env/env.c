/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 20:51:25 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/01/17 16:23:24 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief	Looks through the whole environment.
 * 			Replaces old data of matching key.
 * 
 * @param beans The full beans of the env; "key=data"
 * @param env	The environment variable to be replaced.
 * @returns @b true  When it replaced an env, or the env is illegal.
 * @returns @b false When it did not replace an env.
 */
int	replace_env(char *beans, t_env *env)
{
	if (!env || !env->beans || !beans || ft_strchr_num(beans, '=') == -1)
		return (1);
	free(env->beans);
	env->beans = ft_strdup(beans);
	if (env->key)
		free(env->key);
	if (env->data)
		free(env->data);
	fill_env(env);
	return (0);
}

/**
 * @brief Checks the shells environment for a variable matching @b [key]
 * 
 * @param key	The full key to look for.
 * @param shell The shell.
 * @returns		A pointer to the data of the environment variable @b [key].
 * 				Returns unallocated "" if there is no data matching the key.
 */
char	*ms_getenv(char *key, t_minishell *shell)
{
	t_env	*env;

	env = shell->env_head;
	while (env)
	{
		if (ft_strcmp(key, env->key) == 0)
			return (env->data);
		env = env->next;
	}
	return ("");
}

int	print_env(t_minishell *shell, t_command *cmd)
{
	t_env	*tmp;

	if (!shell->env_head)
		return (1);
	tmp = shell->env_head;
	while (tmp)
	{
		if (tmp && tmp->has_key == true)
		{
			ft_putstr_fd(tmp->beans, cmd->outfile);
			write(cmd->outfile, "\n", 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
