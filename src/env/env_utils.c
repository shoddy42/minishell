/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 03:42:34 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/17 08:53:24 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
