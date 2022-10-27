#include "../include/minishell.h"

int	remove_list(t_env	*env)
{
	printf("In remove\n");
	if(env->beans)
	{
		free(env->beans);
		printf("Freed beans\n");
	}
	if(env->key)
	{
		free(env->key);
		printf("Freed Key \n");
	}
	if(env->data)
	{
		free(env->data);
		printf("Freed data\n");
	}
	return (0);
}

int ms_unset(t_minishell *shell, t_command *cmd)
{
	t_env	*tmp1;
	t_env	*tmp2;

	if(!cmd->command)
		return(1);
	if(!cmd->command[1])
		return(1);
	tmp1 = shell->env;
	while (tmp1)
	{
		if(tmp1->next && ft_strcmp(tmp1->next->key, cmd->command[1]) != 0)
		{
			tmp2 = tmp1->next;
			tmp1->next = tmp2->next;
			if(tmp2)
				printf("TMP2 KEY %s\n", tmp2->key);
			if(!remove_list(tmp2))
				return(0);
		}
		tmp1 = tmp1->next;
	}
	return(0);
}
