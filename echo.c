#include "minishell.h"


char	**get_command_options(t_token	*token, t_command   *cmd)
{
	char	*options;
	t_token	*tmp;
	int		i;

	tmp = token;
	i = 0;

	while (tmp && tmp->type != PIPE)
	{
        if (ft_strcmp(cmd->options[1], 
			i++;
		token = token->next;
	}
	return (options);
}

int ms_echo(t_token *token)
}


}