#include "../include/minishell.h"

int	remove_list(t_env	*env)
{
	if (env->beans)
	{
		free(env->beans);
		env->beans = NULL;
	}
	if (env->key)
	{
		free(env->key);
		env->key = NULL;
	}
	if (env->data)
	{
		free(env->data);
		env->data = NULL;
	}
	free(env);
	return (0);
}

//todo: samething as export, "unset A B C" should unset A=test B=test C=test at the same time.
int ms_unset(t_minishell *shell, t_command *cmd)
{
	t_env	*tmp1;
	t_env	*tmp2;

	if (!cmd->command || !cmd->command[1])
		return (1);
	tmp1 = shell->env;
	if (ft_strcmp(tmp1->key, cmd->command[1]) == 0)
	{
		shell->env = shell->env->next;
		return (remove_list(tmp1));
	}
	while (tmp1 && tmp1->next)
	{
		if (ft_strcmp(tmp1->next->key, cmd->command[1]) == 0)
		{
			tmp2 = tmp1->next;
			if (tmp2->next)
				tmp1->next = tmp2->next;
			if (tmp2)
				return (remove_list(tmp2));
		}
		tmp1 = tmp1->next;
	}
	write(1, cmd->command[1], ft_strlen(cmd->command[1]));
	ft_putstr_fd(" does not exist\n", 1);
	return (0);
}
