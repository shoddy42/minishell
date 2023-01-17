/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: root <root@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 11:43:21 by root          #+#    #+#                 */
/*   Updated: 2023/01/17 17:12:33 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <sys/param.h>

static void	change_pwd(t_minishell *shell, char *env)
{
	char	*pwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin(env, cwd);
	if (!pwd)
		pwd = ft_strdup(ms_getenv("OLDPWD", shell));
	if (!pwd)
		ms_error("getcwd call failed\n", 0, true);
	replace_env(pwd, env_exists(pwd, shell));
	create_envp(shell);
	if (cwd)
		free(cwd);
	free(pwd);
}

static int	go_home(t_minishell *shell)
{
	int	ret;

	change_pwd(shell, "OLDPWD=");
	ret = chdir(ms_getenv("HOME", shell));
	if (ret == -1)
	{
		write (1, "cd: ", 4);
		perror(ms_getenv("HOME", shell));
	}
	change_pwd(shell, "PWD=");
	return (ret * -1);
}

static int	go_back(t_minishell *shell)
{
	char	*back;
	int		ret;

	back = ft_strdup(ms_getenv("OLDPWD", shell));
	if (!back)
		return (1);
	change_pwd(shell, "OLDPWD=");
	ret = chdir(back);
	if (ret == -1)
		perror(ms_getenv("OLDPWD", shell));
	else
		printf ("%s\n", back);
	change_pwd(shell, "PWD=");
	free (back);
	return (ret * -1);
}

int	ms_cd(t_command	*cmd, t_minishell *shell)
{
	int	cd_ret;

	if (!cmd->command[1] || ft_strcmp(cmd->command[1], "~") == 0)
		return (go_home(shell));
	if (cmd->command[1] && ft_strcmp(cmd->command[1], "-") == 0)
		return (go_back(shell));
	change_pwd(shell, "OLDPWD=");
	cd_ret = chdir(cmd->command[1]);
	change_pwd(shell, "PWD=");
	return (cd_ret * -1);
}
