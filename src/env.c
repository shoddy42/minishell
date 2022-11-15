/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 20:51:25 by auzochuk      #+#    #+#                 */
/*   Updated: 2022/11/15 08:26:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	fill_key(t_env	*new)
{
	int	eq;

	eq = ms_strchr(new->beans, '=');
	if (eq)
	{
		new->key = ft_substr(new->beans, 0, (eq));
		if (new->beans[eq + 1])
		{
			new->has_beans = true;
			return (fill_data(new, eq));
		}
		else
		{
			new->has_beans = false;
			return (0);
		}
	}
	new->key = ft_substr(new->beans, 0, ft_strlen(new->beans));
	if (new->key)
		return (0);
	return (1);
}

int	fill_data(t_env	*new, int eq)
{
	if (!eq)
		eq = ms_strchr(new->beans, '=');
	new->data = ft_substr(new->beans, (eq + 1), ft_strlen(new->beans));
	if (new->data)
		return (0);
	ms_error("Failed to allocate ENV->data.", -1, true, NULL);
	return (1);
}

t_env	*new_env(char *data)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		ms_error("Failed to allocate ENV.", -1, true, NULL);
	new->beans = ft_strdup(data);
	if (!new->beans)
		ms_error("Failed to allocate ENV->beans.", -1, true, NULL);
	fill_key(new);
	return (new);
}

//fixed this up so its doesnt replace when not needed.
int	ms_replace_env(char *beans, t_minishell *shell)
{
	t_env	*tmp;

	tmp = shell->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, beans, ft_strchr_num(beans, '=')) == 0)
		{
			// printf ("replacing! key: [%s] beans: [%s]\n", tmp->key, beans);
			free(tmp->beans);
			free(tmp->data);
			tmp->beans = ft_strdup(beans);
			if (!beans)
				ms_error("CANNOT ALLOCATE MORE ENV", -1, false, shell);
			fill_data(tmp, 0);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ms_export_loop(char *command, t_minishell *shell)
{
	t_env	*new;
	t_env	*tmp;

	tmp = shell->env;
	if (!command || !tmp)
		return (1);
	new = new_env(command);
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (0);
}

void	ms_export_env(t_minishell	*shell)
{
	t_env   *tmp;

	if (!shell->env)
		exit(1);
	tmp = shell->env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->key, 1);
		if (tmp->data && ft_strcmp(tmp->data, "") != 0)
		{
			write(1, "=", 1);
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(tmp->data, 1);
			ft_putstr_fd("\"", 1);
		}
		write(1, "\n", 1);
		tmp = tmp->next;
	}
}

int	ms_export(t_command *cmd, t_minishell *shell)
{
	int	i;
	int	j;

	i = 1;
	j = -1;
	// while (cmd->command[++j])
	// 	printf ("cmd(%i) = [%s]\n", j, cmd->command[j]);
	if (!cmd->command[i])
		ms_export_env(shell);
	while (cmd->command[i])
	{
		if (ms_replace_env(cmd->command[i], shell) != EXIT_SUCCESS)
			ms_export_loop(cmd->command[i], shell);
		i++;
	}
	return (0);
}

int	ms_env(t_minishell *shell, t_command *cmd)
{
	t_env	*tmp;

	if (!shell->env)
		return (1);
	tmp = shell->env;
	while (tmp)
	{
		if (tmp && tmp->has_beans == true)
		{
			ft_putstr_fd(tmp->beans, 1);
			write(cmd->outfile, "\n", 1);
		}
		tmp = tmp->next;
	}
	return (0);
}

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

void	init_env(t_minishell *shell, char **env)
{
	int		i;
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
}
