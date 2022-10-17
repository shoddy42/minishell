#include "../include/minishell.h"

int ms_unset(t_minishell *shell, t_command *cmd)
{
	int		i;
	t_env	*tmp1;
	t_env	*tmp2;
	t_env	*tmp3;

	if(!cmd->command)
		return(1);
	tmp1 = shell->env;
	
	tmp3 = tmp2->next;

	while (tmp1)
	{
		if(tmp1->next && ft_strcmp(tmp1->next->key, cmd->command[1]) != 0)
		{
			tmp2 = tmp1->next;
		}
		tmp1 = tmp1->next;
		tmp2 = tmp2->next;
		tmp3 = tmp3->next;
	}
	i = 0;
	return(0);
}