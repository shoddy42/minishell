#include "../include/minishell.h"

int ms_echo(t_command *command)
{
	int	i;
	char	*ret;
	int	x;

	i = 1;
	x = 0;
	if(!(command->options))
		return(1);
	while(command->options[3])
	{
		// if (command->options[i][x] == "\"")
		// 	x++;
		// ret = ft_strjoin(ret, command->options[i]);
		i++;
		printf("Ret = %s", command->options[i]);
	}
	return(0);

}
