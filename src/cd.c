/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: root <root@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 11:43:21 by root          #+#    #+#                 */
/*   Updated: 2022/11/10 03:26:36 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/param.h>


// void	cwd(t_minishell *shell)
// {
// 	char	*pwd;

// 	pwd = getcwd(NULL, 0);
// 	if (pwd)
// 		pwd = ft_strjoin ("PWD=", pwd);
// 	if (pwd)
// 		ms_replace_env(pwd, shell);
// }

// void	owd(t_command	*cmd, t_minishell *shell)
// {
// 	char	*boom;

// 	boom = getcwd(NULL, 0);
// 	if (boom)
// 		boom = ft_strjoin("OLDPWD=", boom);
// 	if (boom)
// 		ms_replace_env(boom, shell);

// }

//can replace owd and cwd
void	wd(t_minishell *shell, char *env)
{
	char	*pwd;

	pwd = ft_strjoin(env, getcwd(NULL, 0));
	if (!pwd)
		ms_error("getcwd call failed\n", 0, true, shell);
	ms_replace_env(pwd, shell);
	free(pwd);
}

// refactored
int	go_home(t_minishell *shell)
{
	return (chdir(ms_getenv("HOME", shell)) * -1);

	// char	*home;
	// int		ret;

	// printf ("going home?\n");
	// home = ms_getenv("HOME", shell);
	// printf ("changing dir to [%s]\n", home);
	
	// ret = chdir(home);
	// return (ret * -1);
	// if (ret < 0)
	// 	return (1);
	// return (0);
}

int	go_back(t_minishell *shell)
{
	char	*back;
	int		ret;
	
	back = ms_getenv("OLDPWD", shell);
	printf ("%s\n", back);
	return (chdir(back) * -1);
	// ret = chdir(back);
	// return (ret * -1);
	// if (ret < 0)
	// 	return (1);
	// return (0);
}

//replaced cd
int	ms_cd(t_command	*cmd, t_minishell *shell)
{
	int	cd_ret;

	if (!cmd->command[1] || (cmd->command[1] && ft_strcmp(cmd->command[1], "~") == 0))
		return (go_home(shell));
	if (cmd->command[1] && ft_strcmp(cmd->command[1], "-") == 0)
		return (go_back(shell));
	wd(shell, "OLDPWD=");
	cd_ret = chdir(cmd->command[1]);
	// printf ("cd ret = [%i]\n", cd_ret);
	wd(shell, "PWD=");
	return (cd_ret * -1);
	// if (cd_ret < 0)
	// 	return (1);
	// return (0);
}
