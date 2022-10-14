#include "../include/minishell.h"

int ms_echo(t_command *command)
{
	int	i;
	char	*ret;
	int	x;

	i = 1;
	x = 0;
	if(!(command->command))
		return(1);
	while(command->command[3])
	{
		// if (command->command[i][x] == "\"")
		// 	x++;
		// ret = ft_strjoin(ret, command->command[i]);
		i++;
		printf("Ret = %s", command->command[i]);
	}
	return(0);

}
