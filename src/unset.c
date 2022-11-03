#include "../include/minishell.h"

int	remove_list(t_env	*env)
{
	if (env->beans)
		free(env->beans);
	if (env->key)
		free(env->key);
	if (env->data)
		free(env->data);
	free(env);
	return (0);
}

int ms_unset_loop(t_minishell *shell, char *cmd, t_env *tmp)
{
	t_env	*tmp1;
	t_env	*tmp2;

	printf("HERE\n");
	if (!cmd || !tmp)
		return (1);
	if (tmp->next)
		printf("TMPS NEXT: %s\n", tmp->next->key);
	if (tmp->next)
		tmp2 = tmp->next;
	tmp->next = tmp2->next;
	remove_list(tmp2);
	// write(1, cmd, ft_strlen(cmd));
	// ft_putstr_fd(" does not exist\n", 1);
	return (0);
}

int	ms_unset(t_minishell *shell, t_command *cmd)
{
	int		i;
	t_env	*tmp;

	tmp = shell->env;
	i = 1;
	if (ft_strcmp(tmp->key, cmd->command[i]) == 0)
	{
		shell->env = shell->env->next;
		return (remove_list(tmp));
	}
	while (cmd->command[i])
	{
		while (tmp && tmp->next)
		{
			if (ft_strcmp(cmd->command[i], tmp->next->key) == 0)
				ms_unset_loop(shell, cmd->command[i], tmp);
			tmp = tmp->next;
		}
		tmp = shell->env;
		i++;
	}
	return (0);
}