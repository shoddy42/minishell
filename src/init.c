/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 09:24:40 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/23 18:27:12 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_sig(int signum)
{
	extern int	g_status;

	if (signum == SIGQUIT)
	{
		g_status = 131;
	}
	if (signum == SIGINT)
	{
		g_status = 130;
		rl_redisplay();
	}
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	// exit(1); //later: remove
}

char	**bin_command(t_minishell *shell)
{
	char	**dir;

	dir = ft_calloc(4, sizeof(char *));
	if (!dir)
		return (NULL);
	if (errno == EACCES)
	{
		printf ("access issue!\n");
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
			printf ("%s FAILED!\n", bin_cmd[0]);
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

void	increase_shlvl(t_minishell *shell)
{
	char	*shlvl;
	char	*num;

	num = ft_itoa(1 + ft_atoi(ms_getenv("SHLVL", shell)));
	shlvl = ft_strjoin("SHLVL=", num);
	replace_env(shlvl, env_exists(shlvl, shell), shell);
	free(shlvl);
	free(num);
}

void	init_env(t_minishell *shell, char **envp)
{
	int		i;
	t_env	*tmp;

	i = 0;
	while (envp[++i])
		new_env(envp[i], shell);
	create_envp(shell);
	replace_env("SHELL=minishell", env_exists("SHELL=minishell", shell), shell);
	increase_shlvl(shell);
}

int	init_minishell(t_minishell *shell, char **envp)
{
	extern int	g_status;

	if (signal(SIGINT, sighandler) == SIG_ERR)
		exit (55);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		exit (56);
	shell->cancel_all_commands = false;
	g_status = 0;
	shell->bin_dir = getcwd(NULL, 0);
	shell->bin_dir = ft_strexpand(shell->bin_dir, "/bin");
	init_env(shell, envp);
	return (0);
}
