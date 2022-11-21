/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 09:24:40 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/21 17:29:00 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_sig(int signum)
{
	if (signum == SIGINT)
		rl_redisplay();
	// if (signum == SIGQUIT) useless because unless sigquit is ignored, it just quits...
	// {
	// 	exit (3);
	// }
}

void	sighandler(int signum)
{
	if (signum == SIGINT) // ctrl + C
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	exit(1); //later: remove
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
	// signal handlers
	if (signal(SIGINT, sighandler) == SIG_ERR)
		exit (55);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		exit (56);
	// environment setup
	shell->cancel_all_commands = false;
	shell->last_return = 0;
	shell->bin_dir = getcwd(NULL, 0);
	shell->bin_dir = ft_strexpand(shell->bin_dir, "/bin/");
	init_env(shell, envp);
	return (0);
}
