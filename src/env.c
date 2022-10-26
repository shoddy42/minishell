/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 00:57:02 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/27 00:33:27 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*fill_key(char	*beans)
{
	int		eq;
	char	*ret;
	
	eq = ms_strchr(beans, '=');
	if (!eq)
		return(NULL);
	ret = ft_substr(beans, 0, (eq));
	return(ret);
}

char	*fill_data(char	*beans)
{
	int		eq;
	char	*ret;

	eq = ms_strchr(beans, '=');
	if (!(eq))
		return (NULL);
	ret = ft_substr(beans, (eq + 1), ft_strlen(beans));
	return(ret);
}

t_env	*new_env(char *data)
{
	t_env *new;

	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		ms_error("Failed to allocate ENV." -1);
	new->beans = ft_strdup(data);
	if (!new->beans)
		ms_error("Failed to allocate ENV->beans." -1);
	new->data = fill_data(new->beans);
	if (!new->data)
		ms_error("Failed to allocate ENV->data." -1);
	new->key = fill_key(new->beans);
	if (!new->data)
		ms_error("Failed to allocate ENV->key." -1);
	return (new);
}

//todo: Make export work with quotationmarks like [export XD="lol funny"]
//todo: Make it so we dont have duplicates, and instead overwrite the data.

int	ms_export(t_command *cmd, t_minishell	*shell)
{
	int		i;
	int		eq;
	int		len;
	t_env	*new;
	t_env	*tmp;

	i = 0;
	eq = 0;
	len = 0;
	tmp = shell->env;
	if(!cmd->command[1])
		return (1);
	eq = ms_strchr(cmd->command[1], '=');
	if (!eq)
		return(1);
	new = new_env(cmd->command[1]);
	new->beans = ft_strdup(cmd->command[1]);
	new->key = fill_key(new->beans);
	new->data = fill_data(new->beans);
	while(tmp && tmp->next)
	{
		if (ft_strcmp(tmp->key, new->key) == 0)
		{
			new->
		}
		tmp = tmp->next;
	}
	tmp->next = new;
	return(0);
}

int	ms_env(t_minishell	*shell, t_command *cmd)
{
	t_env	*tmp;

	if(!(shell->env))
		return(1);
	tmp = shell->env;
	while(tmp)
	{
		ft_putstr_fd(tmp->beans,1); 
		write(cmd->outfile, "\n", 1);
		tmp = tmp->next;
	}
	return(0);
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
}


