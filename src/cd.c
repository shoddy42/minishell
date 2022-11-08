#include "../include/minishell.h"
#include <sys/param.h>

int	go_home(t_command	*cmd)
{
	int		cd_ret;
	char	*home_p;

	if (!(home_p = getenv("HOME=")))
		return (1);
	cd_ret = chdir(home_p);
	return (0);
}

void	cwd(t_minishell *shell)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if(pwd)
		pwd = ft_strjoin ("PWD=", pwd);
	if (pwd)
		ms_replace_env(pwd, shell);
}

void	owd(t_command	*cmd, t_minishell *shell)
{
	char	*boom;

	boom = getcwd(NULL, 0);
	if (boom)
		boom = ft_strjoin("OLDPWD=", boom);
	if (boom)
		ms_replace_env(boom, shell);
	
}

//todo: make go_home dependant on the env HOME. if HOME is unset, "cd", should not take us to root.
int	ms_cd(t_command	*cmd, t_minishell *shell)
{
	int	cd_ret;

	if (!cmd->command)
		return (1);

	owd(cmd, shell);
	cd_ret = chdir(cmd->command[1]);
	cwd(shell);
	if (cd_ret == 1)
		return (1);
	if (!cmd->command[1])
		return (go_home(cmd));
	else return (0);
}

/*
WIP
*/