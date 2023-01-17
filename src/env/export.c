/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/22 19:11:04 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:35:56 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_export(t_minishell	*shell)
{
	t_env	*env;

	env = shell->env_head;
	if (!env)
		exit(1);
	while (env)
	{
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

int	ms_export(t_command *cmd, t_minishell *shell)
{
	t_env	*env;
	int		status;
	int		i;

	if (!cmd->command[1])
		print_export(shell);
	i = 0;
	status = 0;
	while (cmd->command[++i])
	{
		env = env_exists(cmd->command[i], shell);
		if (env)
			status = replace_env(cmd->command[i], env);
		else
			status = new_env(cmd->command[i], shell);
	}
	create_envp(shell);
	return (status);
}
