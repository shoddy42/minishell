/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: root <root@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 11:43:21 by root          #+#    #+#                 */
/*   Updated: 2022/11/17 08:53:24 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <sys/param.h>


//todo: find leak in cd?
void	change_pwd(t_minishell *shell, char *env)
{
	char	*pwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin(env, cwd);
	if (!pwd)
		ms_error("getcwd call failed\n", 0, true, shell);
	ms_replace_env(pwd, shell);
	free(cwd);
	free(pwd);
}

int	go_home(t_minishell *shell)
{
	int	ret;

	change_pwd(shell, "OLDPWD=");
	ret = chdir(ms_getenv("HOME", shell));
	change_pwd(shell, "PWD=");
	return (ret);
}

int	go_back(t_minishell *shell)
{
	char	*back;
	int		ret;

	back = ft_strdup(ms_getenv("OLDPWD", shell));
	if (!back)
		return (1);
	printf ("%s\n", back);
	change_pwd(shell, "OLDPWD=");
	ret = chdir(back);
	change_pwd(shell, "PWD=");
	free (back);
	return (ret * -1);
}

int	ms_cd(t_command	*cmd, t_minishell *shell)
{
	int	cd_ret;

	if (!cmd->command[1] || ft_strcmp(cmd->command[1], "~") == 0)
		return (chdir(ms_getenv("HOME", shell)) * -1);
	if (cmd->command[1] && ft_strcmp(cmd->command[1], "-") == 0)
		return (go_back(shell));
	change_pwd(shell, "OLDPWD=");
	cd_ret = chdir(cmd->command[1]);
	change_pwd(shell, "PWD=");
	return (cd_ret * -1);
}
