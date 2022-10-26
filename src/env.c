/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 00:57:02 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/26 01:06:47 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*new_env(char *data)
{
	t_env *new;

	new = ft_calloc(1, sizeof(t_env));
	new->beans = ft_strdup(data);
	return (new);
}

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
		tmp = tmp->next;
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

void	init_env(t_minishell *shell, char  **env)
{
	int i;
	t_env	*tmp;

	i = 0;
	shell->env = new_env(env[i]);
	shell->env->key = fill_key(env[i]);
	shell->env->data = fill_data(env[i]);
	tmp = shell->env;
	while (env[++i])
	{
		tmp->next = new_env(env[i]);
		shell->env->key = fill_key(env[i]);
		shell->env->data = fill_data(env[i]);
		// printf("Data: %s\n",shell->env->data);
		tmp = tmp->next;

	}
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5) != 0 && env[i + 1])
		i++;
	if (ft_strncmp(env[i], "PATH=", 5) != 0)
		exit (1);
	shell->path = ft_split(env[i] + 6, ':');
}


