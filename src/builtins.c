#include "../include/minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int	check_qoutes(char *str)
{
	int		i;
	size_t	x;

	i = 0;
	while(str[i + 1] != '\0')
		i++;
	printf("X IN THE STRING IS [%s]\n", str);
	if(!(ft_strcmp(&str[i], "\"")))
		printf("GREAT SUCKSESS\n");
		return(0);
	return(1);

	
}

int ms_echo(t_command *command)
{
	int	i;
	char	*ret;
	int	x;

	i = 0;
	x = 0;
	printf("str[i] = [%s]\n", command->options[2]);
	if(!(command->options))
		return(1);
	while(command->options[i])
	{
		// if (command->options[i][x] == "\"")
		// 	x++;
		// ret = ft_strjoin(ret, command->options[i]);
		i++;
	}
	check_qoutes(command->options[i - 1]);
	return(0);

}


int check_builtin(t_command    *cmd)
{
	if(strcmp(cmd->options[0], "cd") == 0)
		return(ms_cd(cmd));
	else if(ft_strcmp(cmd->options[0], "echo") == 0)
		return(ms_echo(cmd));
	// else if(ft_strcmp(cmd->options[0], "env") == 0)
	//     ms_env(cmd);
	// else if(ft_strcmp(cmd->options[0], "pwd") == 0)
	//     ms_pwd(cmd);
	// else if(ft_strcmp(cmd->options[0], "cd") == 0)
	//     ms_export(cmd);
	// else if(ft_strcmp(cmd->options[0], "unset") == 0)
	//     ms_exit(cmd);
	// else if(ft_strcmp(cmd->options[0], "exit") == 0)
	//     ms_exit(cmd);
	else   
		return(1);
	return (0);
}

