/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 20:51:25 by auzochuk      #+#    #+#                 */
/*   Updated: 2022/11/19 00:31:53 by wkonings      ########   odam.nl         */
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
bool	replace_env(char *beans, t_env *env, t_minishell *shell)
{
	if (!env || !env->beans || !beans || ft_strchr_num(beans, '=') == -1)
		return (false);
	free(env->beans);
	env->beans = ft_strdup(beans);
	if (env->key)
		free(env->key);
	if (env->data)
		free(env->data);
	fill_key(env);
	return (true);
}

//todo: maybe add +=
int	ms_export(t_command *cmd, t_minishell *shell)
{
	t_env	*env;
	int		i;

	if (!cmd->command[1])
		print_export(shell);
	i = 0;
	while (cmd->command[++i])
	{
		env = env_exists(cmd->command[i], shell);
		if (env)
		{
			printf ("[%s] exists\n", cmd->command[i]);
			replace_env(cmd->command[i], env, shell);
		}
		else
		{
			printf ("[%s] DOESNT exist\n", cmd->command[i]);
			new_env(cmd->command[i], shell);
		}
	}
	create_envp(shell);
	// print_envp(shell->envp);
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

	env = shell->env;
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

	if (!shell->env)
		return (1);
	tmp = shell->env;
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

void	print_export(t_minishell	*shell)
{
	t_env	*env;

	env = shell->env;
	if (!env)
		exit(1);
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->data && env->has_key == true)
		{
			write(1, "=", 1);
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(env->data, 1);
			ft_putstr_fd("\"", 1);
		}
		write(1, "\n", 1);
		env = env->next;
	}
}
