/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 03:42:34 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/18 23:00:10 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Looks through all environment variables and checks if beans exists.
 * 
 * @param beans The string to check against the envs.
 * @param shell The shell.
 * @returns The found env if one exists. 
 * @returns NULL if it doesn't find one.
 */
t_env	*env_exists(char *beans, t_minishell *shell)
{
	t_env	*env;
	int		len;

	len = ft_strchr_num(beans, '=');
	if (len == -1)
		len = ft_strlen(beans);
	env = shell->env;
	while (env)
	{
		if (ft_strncmp(beans, env->key, len) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

bool	legal_env(char *data)
{
	int			i;
	const char	legal[] = "_=";
	const char	legal_last[] = "+";
	const char	illegal[] = "~@#%%^&*()-|[]{};:\"\'$";
	const char	first_illegal[] = "+=";

	if (!data || (ft_isalpha(data[0]) == false && data[0] != '_'))
	{
		printf ("export: `%s': not a valid identifier.\n", data);
		return (false);
	}
	i = -1;
	while (data[++i] && data[i] != '=')
	{
		if (ft_isalnum(data[i]) == false && data[i] != '_' && data[i] != '=')
		{
			printf ("export: `%s': not a valid identifier.\n", data);
			return (false);
		}	
	}
	return (true);
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
	env = shell->env;
	while (env && env->next)
		env = env->next;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		ms_error("Failed to allocate ENV.", -1, true, NULL);
	new->beans = ft_strdup(data);
	if (!new->beans)
		ms_error("Failed to allocate ENV->beans.", -1, true, NULL);
	printf ("new = [%s]\n", new->beans);
	fill_key(new);
	if (!env)
		shell->env = new;
	else
		env->next = new;
}

// void	print_envp(char **envp)
// {
// 	int	i;

// 	i = -1;
// 	printf ("\nPRINTING NEW ENVP \n");
// 	while (envp[++i])
// 	{
// 		printf("%s\n", envp[i]);
// 	}
// 	printf ("\n");
// }

//todo: change envp creation and freeing
//		to whenever the env linked list changes.
char	**create_envp(t_env *env_head)
{
	t_env	*env;
	char	**ret;
	int		size;
	int		i;

	size = 0;
	env = env_head;
	while (env)
	{
		size++;
		env = env->next;
	}
	env = env_head;
	ret = ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		printf ("major error\n");
	i = 0;
	while (env)
	{
		ret[i++] = env->beans;
		env = env->next;
	}
	return (ret);
}
