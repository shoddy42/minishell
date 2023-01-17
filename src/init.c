/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 09:24:40 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:25:59 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**bin_command(t_minishell *shell)
{
	char	**dir;

	dir = ft_calloc(4, sizeof(char *));
	if (!dir)
		return (NULL);
	if (errno == EACCES)
	{
		dir[0] = "/bin/chmod";
		dir[1] = "777";
		dir[2] = shell->bin_dir;
	}
	else
	{
		dir[0] = "/bin/mkdir";
		dir[1] = shell->bin_dir;
	}
	return (dir);
}

void	create_bin(t_minishell *shell)
{
	pid_t	pid;
	char	**bin_cmd;

	shell->bin = opendir(shell->bin_dir);
	bin_cmd = bin_command(shell);
	if (!shell->bin && bin_cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			execve(bin_cmd[0], bin_cmd, shell->envp);
			exit (1);
		}
		if (pid > 0)
			waitpid(0, NULL, 0);
	}
	else
		closedir(shell->bin);
	if (bin_cmd)
		free (bin_cmd);
}

static void	increase_shlvl(t_minishell *shell)
{
	char	*shlvl;
	char	*num;

	num = ft_itoa(1 + ft_atoi(ms_getenv("SHLVL", shell)));
	shlvl = ft_strjoin("SHLVL=", num);
	replace_env(shlvl, env_exists(shlvl, shell));
	free(shlvl);
	free(num);
}

static void	init_env(t_minishell *shell, char **envp)
{
	int		i;

	i = -1;
	while (envp[++i])
		new_env(envp[i], shell);
	create_envp(shell);
	replace_env("SHELL=minishell", env_exists("SHELL=minishell", shell));
	increase_shlvl(shell);
	create_envp(shell);
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*shell;
	extern int	g_status;

	shell = ft_calloc(1, sizeof(t_minishell));
	if (!shell)
		ms_error(SHELL_ALLOC_FAILURE, false, true);
	shell->prompt = "WELCOME TO MINIS HELL > ";
	if (signal(SIGINT, sighandler) == SIG_ERR)
		exit (55);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		exit (56);
	shell->cancel_all_commands = false;
	g_status = 0;
	shell->bin_dir = getcwd(NULL, 0);
	shell->bin_dir = ft_strexpand(shell->bin_dir, "/bin");
	init_env(shell, envp);
	return (shell);
}
