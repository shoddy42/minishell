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


// int	owd(t_command	*cmd)
// {
// 	char	cwd[PATH_MAX];
// 	char	*last_wd;

// 	if(getcwd(cwd, PATH_MAX) == NULL)
// 		return (1);
// 	last_wd = ft_strjoin("LAST_PATH=", last_wd)
	
// }


//todo: make go_home dependant on the env HOME. if HOME is unset, "cd", should not take us to root.
//todo:? make it so cd sets the ENV PWD, and OLDPWD usure if neccesary.
int    ms_cd(t_command	*cmd)
{
	int cd_ret;

	if(!cmd->command)
		return(1);
	cd_ret = chdir(cmd->command[1]);
	if (cd_ret == 1)
		return (1);
	if (!cmd->command[1])
		return(go_home(cmd));
	else return (0);
}

/*
WIP
*/