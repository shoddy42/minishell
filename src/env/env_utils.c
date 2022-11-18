/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 03:42:34 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/18 20:38:04 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

bool	legal_env(char *data)
{
	int	i;
	char legal[] = "_=";
	char legal_last[] = "+";
	char illegal[] = "~@#%%^&*()-|[]{};:\"\'$";
	char first_illegal[] = "+=";

	if (!data || (ft_isalpha(data[0]) == false && data[0] != '_')) //block all illegal characters.
	{
		printf ("export: `%s': not a valid identifier.\n", data);
		printf ("ILLEGAL FIRST CHAR [%s] c = [%c]\n", data, data[0]);
		return (false);
	}
	i = -1;
	while (data[++i] && data[i] != '=') //skim until key
	{
		if (ft_isalnum(data[i]) == false && data[i] != '_' && data[i] != '=') //block all illegal characters
		{
			printf ("export: `%s': not a valid identifier.\n", data);
			printf ("ILLEGAL ENV [%s] c = [%c]\n", data, data[i]);
			return (false);
		}	
	}
	return (true);
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

void	print_envp(char **envp)
{
	int	i;

	i = -1;
	printf ("\nPRINTING NEW ENVP \n");
	while (envp[++i])
	{
		printf("%s\n", envp[i]);
	}
	printf ("\n");
}

//todo: change envp creation and freeing to whenever the env linked list changes.
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
