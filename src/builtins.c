#include "../include/minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>


//todo: make ms_exit actually get an exit value.
void	ms_exit(t_minishell *shell)
{
	exit (1);
}

//todo: make all builtins always have the correct exit status for the purposes of $?
//^ use shell->last_return for this

//todo: cd -, should display the value of $OLDPWD. which means we should track and update $OLDPWD and $PWD
int check_builtin(t_command *cmd, t_minishell *shell, t_from process)
{
	if (cmd->command && cmd->command[0])
	{
		if(ft_strcmp(cmd->command[0], "cd") == 0)
			return(ms_cd(cmd, shell));
		else if(ft_strcmp(cmd->command[0], "echo") == 0)
			return(ms_echo(cmd));
		else if(ft_strcmp(cmd->command[0], "env") == 0)
			return(ms_env(shell, cmd));
		else if(ft_strcmp(cmd->command[0], "pwd") == 0)
			ms_pwd(cmd);
		else if(ft_strcmp(cmd->command[0], "export") == 0)
			ms_export(cmd, shell);
		else if(ft_strcmp(cmd->command[0], "unset") == 0)
			ms_unset(shell, cmd);
		else if(ft_strcmp(cmd->command[0], "exit") == 0 && process != CHILD)
			ms_exit(shell);
		else
			return(1);
	}
	return (0);
}

int	ms_strchr(const char *src, int c)
{
	int		i;

	i = 0;
	while (src[i])
	{
		if (src[i] == (char)c)
			return (i);
		i++;
	}
	if (src[i] == (char)c)
		return (i);
	return (0);
}
