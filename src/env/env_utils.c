/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 03:42:34 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 17:24:11 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//returns a pointer to an env matching. expects a key including the '='
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

//returns whether an env key is legal or not.
static bool	legal_env(char *data)
{
	bool	ret;
	int		i;

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

int	fill_env(t_env	*new)
{
	int	eq;

	eq = ft_strchr_num(new->beans, '=');
	if (eq > 0)
	{
		new->has_key = true;
		new->key = ft_substr(new->beans, 0, (eq));
		new->data = ft_substr(new->beans, (eq + 1), ft_strlen(new->beans) - eq);
		if (!new->key || !new->data)
			ms_error("Failed to allocate ENV.", 0, true);
		return (0);
	}
	new->key = ft_strdup(new->beans);
	new->data = ft_strdup("");
	return (0);
}

int	new_env(char *data, t_minishell *shell)
{
	t_env	*env;
	t_env	*new;

	if (legal_env(data) == false)
	{
		ms_error("Illegal env", 0, false);
		return (1);
	}
	env = shell->env_head;
	while (env && env->next)
		env = env->next;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		ms_error("Failed to allocate ENV.", -1, true);
	new->beans = ft_strdup(data);
	if (!new->beans)
		ms_error("Failed to allocate ENV->beans.", -1, true);
	fill_env(new);
	if (!env)
		shell->env_head = new;
	else
		env->next = new;
	return (0);
}
