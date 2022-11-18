/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 20:51:25 by auzochuk      #+#    #+#                 */
/*   Updated: 2022/11/18 21:04:57 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		if (env->data && ft_strcmp(env->data, "") != 0)
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

/**
 * @brief	Looks through the whole environment.
 * 			Replaces old data of matching key.
 * 
 * @param beans The full beans of the env; "key=data"
 * @param shell The shell.
 * @returns @b true  When it replaced an env, or the env is illegal.
 * @returns @b false When it did not replace an env.
 */
bool	ms_replace_env(char *beans, t_minishell *shell)
{
	t_env	*tmp;

	tmp = shell->env;
	if (ft_strchr_num(beans, '=') != -1)
		return (false);
	while (tmp)
	{
		if (ft_strncmp(tmp->key, beans, ft_strlen(tmp->key)) == 0)
		{
			printf ("replacing! key: [%s] beans: [%s]\n", tmp->key, beans);
			free(tmp->beans);
			free(tmp->data);
			tmp->beans = ft_strdup(beans);
			if (!beans)
				ms_error("CANNOT ALLOCATE MORE ENV", -1, false, shell);
			fill_data(tmp, 0);
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

int	ms_export_loop(char *command, t_minishell *shell)
{
	t_env	*new;
	t_env	*tmp;

	tmp = shell->env;
	if (!command || !tmp)
		return (1);
	printf ("Attempting to create env: [%s]\n", command);
	new = new_env(command);
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = new;
	return (0);
}

bool	env_exists(char	*beans, t_minishell *shell)
{
	t_env *env;

	env = shell->env;
	if (!env || ft_strchr_num(beans, '='))
		return (false);
	while (env)
	{
		if (ft_strncmp())
	}
}

int	exoprt(char *beans, t_minishell *shell)
{
	t_env	*env;

	
}

int	ms_export(t_command *cmd, t_minishell *shell)
{
	int	i;
	int	j;

	i = 1;
	j = -1;
	if (!cmd->command[i])
		print_export(shell);
	while (cmd->command[i])
	{
		if (legal_env(cmd->command[i]))
			if (ms_replace_env(cmd->command[i], shell) == false)
				ms_export_loop(cmd->command[i], shell);
		i++;
	}
	return (0);
}

int	print_env(t_minishell *shell, t_command *cmd)
{
	t_env	*tmp;

	if (!shell->env)
		return (1);
	tmp = shell->env;
	while (tmp)
	{
		if (tmp && tmp->has_beans == true)
		{
			ft_putstr_fd(tmp->beans, cmd->outfile);
			write(cmd->outfile, "\n", 1);
		}
		tmp = tmp->next;
	}
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
