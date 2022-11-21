/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 03:42:34 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/21 16:21:27 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*env_exists(char *beans, t_minishell *shell)
{
	t_env	*env;
	int		len;

	len = ft_strchr_num(beans, '=');
	if (len == -1)
		len = ft_strlen(beans);
	env = shell->env_head;
	while (env)
	{
		if (ft_strncmp(beans, env->key, len) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

// const char	legal[] = "_=";
// const char	legal_last[] = "+";
// const char	illegal[] = "~@#%%^&*()-|[]{};:\"\'$";
// const char	first_illegal[] = "+=";
bool	legal_env(char *data)
{
	int	i;
	bool	ret;

	ret = true;
	if (!data || (ft_isalpha(data[0]) == false && data[0] != '_'))
		ret = false;
	i = -1;
	while (data[++i] && data[i] != '=')
		if (ft_isalnum(data[i]) == false && data[i] != '_' && data[i] != '=')
			ret = false;
	if (ret == false)
		printf ("export: `%s': not a valid identifier.\n", data);
	return (ret);
}

int	fill_key(t_env	*new)
{
	int	eq;

	eq = ft_strchr_num(new->beans, '=');
	if (eq > 0)
	{
		new->has_key = true;
		new->key = ft_substr(new->beans, 0, (eq));
		new->data = ft_substr(new->beans, (eq + 1), ft_strlen(new->beans) - eq);
		if (!new->key || !new->data)
			ms_error("Failed to allocate ENV.", 0, true, NULL);
		return (0);
	}
	new->key = ft_strdup(new->beans);
	new->data = ft_strdup("");
	return (0);
}

void	new_env(char *data, t_minishell *shell)
{
	t_env	*env;
	t_env	*new;

	if (legal_env(data) == false)
	{
		ms_error("Illegal env", 0, false, shell);
		return ;
	}
	env = shell->env_head;
	while (env && env->next)
		env = env->next;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		ms_error("Failed to allocate ENV.", -1, true, NULL);
	new->beans = ft_strdup(data);
	if (!new->beans)
		ms_error("Failed to allocate ENV->beans.", -1, true, NULL);
	// printf ("new = [%s]\n", new->beans);
	fill_key(new);
	if (!env)
		shell->env_head = new;
	else
		env->next = new;
}

void	create_path(t_minishell *shell)
{
	int		i;

	i = -1;
	if (shell->path)
		while (shell->path[++i])
			free(shell->path[i]);
	if (shell->path)
	{
		free(shell->path);
		shell->path = NULL;
	}
	i = -1;
	while (shell->envp[++i])
		if (ft_strncmp("PATH=", shell->envp[i], 5) == 0)
			shell->path = ft_split(shell->envp[i] + 6, ':');
	// if (!shell->path)
	// 	printf ("NO PATH\n");
}

void	create_envp(t_minishell *shell)
{
	t_env	*env;
	int		size;
	int		i;

	size = 0;
	env = shell->env_head;
	while (env)
	{
		size++;
		env = env->next;
	}
	env = shell->env_head;
	if (shell->envp)
		free(shell->envp);
	shell->envp = ft_calloc(size + 1, sizeof(char *));
	if (!shell->envp || !env)
		printf ("major error\n");
	i = 0;
	while (env)
	{
		shell->envp[i++] = env->beans;
		env = env->next;
	}
	create_path(shell);
}
